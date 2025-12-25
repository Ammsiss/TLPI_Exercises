#define _GNU_SOURCE

#include <signal.h>
#include <mqueue.h>

#include "tlpi_hdr.h" // IWYU pragma: export

#define MAX_MSG_SIZE 4096

#define USER_PMSG_TEMPLATE "/chat_usr_pmsg.%d"
#define USER_PMSG_NAME_LEN (sizeof(USER_PMSG_TEMPLATE) + 20)

char pmsg_name[USER_PMSG_NAME_LEN];

static void unlink_pmsg(void)
{
    if (mq_unlink(pmsg_name) == -1)
        errMsg("mq_unlink");
}

static volatile sig_atomic_t quit = 0;

/* Catch SIGINT and SIGTERM so that we can unlink our pmsg */
static void sig_catcher(int sig)
{
    (void) sig;

    quit = 1;
}

static volatile sig_atomic_t msg_arrived = 0;

static void pmsg_notify(int sig)
{
    (void) sig;

    msg_arrived = 1;
}

static char* get_input(char *buf, char *prompt)
{
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, MAX_MSG_SIZE, stdin) == NULL) {
        printf("\n");   /* Put cursor on new line */
        return NULL;
    }

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    return buf;
}

static void process_msg(mqd_t mqd)
{
    static char in_msg[MAX_MSG_SIZE];
    int num_read;

    if ((num_read = mq_receive(mqd, in_msg, MAX_MSG_SIZE, NULL)) == -1) {
        errMsg("Failed to read msg. (^C to quit)");
        return;
    }

    printf("%s\n", in_msg);
}

int main(int argc, char **argv)
{
    if (argc > 2 || (argc == 2 && strcmp("--help", argv[1]) == 0))
        usageErr("%s [peer-pid]", argv[0]);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_catcher;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        errExit("sigaction");

    /* Async handle in messages */
    sa.sa_handler = pmsg_notify;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");


    /* Create pmsg for recieving messages */

    printf("Your pid: %d\n", getpid());

    snprintf(pmsg_name, USER_PMSG_NAME_LEN, USER_PMSG_TEMPLATE,
            getpid());

    int mq_flags = O_RDONLY | O_CREAT | O_EXCL;

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;

    mqd_t mqd = mq_open(pmsg_name, mq_flags, 0600, &attr);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");
    if (atexit(unlink_pmsg) != 0)
        errExit("atexit");

    /* Get peer's pid to create peer's pmsg */

    int peer_pid;

    if (argc == 2)
        peer_pid = getInt(argv[1], GN_NONNEG, "peer-pid");
    else {
        char user_input[20];   /* More then enough space for a pid */

        if (!get_input(user_input, "Enter peer's pid: "))
            fatal("get_input");

        peer_pid = getInt(user_input, GN_NONNEG, "peeripid");
    }

    char peer_pmsg_name[USER_PMSG_NAME_LEN];
    snprintf(peer_pmsg_name, USER_PMSG_NAME_LEN, USER_PMSG_TEMPLATE,
            peer_pid);

    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigaddset(&block_set, SIGTERM);

    struct timespec ts = { /*tv_sec*/ 1, /*tv_nsec=*/ 0 };

    int peer_mqd;

    printf("%s - connecting", peer_pmsg_name);
    fflush(stdout);

    /* open in nonblocking mode so mq_recieve and mq_send can't be
     * interuptted by our signal handlers */

    mq_flags = O_WRONLY | O_NONBLOCK;
    while ((peer_mqd = mq_open(peer_pmsg_name, mq_flags)) == (mqd_t) -1) {

        /* For a visual clue that connecting is in process */
        putchar('.');
        fflush(stdout);

        /* Block for 1 second then try again */
        if (sigtimedwait(&block_set, NULL, &ts) != -1) {
            printf("\n");
            fatal("SIGINT or SIGTERM caught");
        }
    }

    printf("\nSuccessfully opened peer's pmsg.\n");

    char out_msg[MAX_MSG_SIZE];

    /* Careful of race condition where message comes in the handler is invoked
     * and during that time another message comes in. Then when the process
     * re-registers for notifications the queue was not previously emptied and
     * therefore will not get notified of that message or any future ones. */

    struct sigevent se;
    se.sigev_notify = SIGEV_SIGNAL;
    se.sigev_signo = SIGUSR1;

    if (mq_notify(mqd, &se) == -1)
        errExit("mq_notify");

    while (1) {
        if (!get_input(out_msg, "> ")) {
            if (quit) {
                break;
            } else if (msg_arrived) {
                process_msg(mqd);
                if (mq_notify(mqd, &se) == -1)
                    errExit("mq_notify");

                msg_arrived = 0;
                continue;
            } else
                fatal("getinput");
        }

        if (mq_send(peer_mqd, (char *) &out_msg, strlen(out_msg) + 1, 0) == -1)
            errMsg("Failed to send msg. (^C to quit)");   /* +1 for '\0' */
    }

    mq_close(mqd);
    return EXIT_SUCCESS;
}
