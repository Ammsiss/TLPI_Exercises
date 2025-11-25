#define _GNU_SOURCE

#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

#include "../lib/error_functions.h"
#include "../lib/print_rlimit.h"

/************************* RLIMIT_AS Test *************************/

void recurse_test(int count)
{
    if (count == 0)
        return;

    --count;
    recurse_test(count);
}

void test_rlimit_as()
{
    printRlimit("RLIMIT_AS Before:", RLIMIT_AS);

    if (sbrk(1000000) == (void *) -1)   /* Allocate 1 million bytes... */
        errExit("sbrk");

    struct rlimit limit;
    limit.rlim_cur = 100000;
    limit.rlim_max = 100000;
    if (setrlimit(RLIMIT_AS, &limit) == -1) /* ... Then set the soft limit lower then that */
        errExit("setrlimit");

    printRlimit("RLIMIT_AS After:", RLIMIT_AS);

    /* Doesn't immediately fail so try allocate more memory (this failed for me) */
    if (sbrk(1000000) == (void *) -1)
        errExit("sbrk");

    /* Also try and recurse 100 stack frames (this also failed for me) */
    recurse_test(10000);
}

/************************* RLIMIT_CPU Test *************************/

void cpu_limit_handler(int sig)
{
    (void) sig;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    printf("[sec: %ld, us: %ld] Caught SIGXCPU!\n", tv.tv_sec, tv.tv_usec);
}

static volatile sig_atomic_t alarm_trig = 0;

void alarm_handler(int sig)
{
    (void) sig;

    alarm_trig = 1;
}

void test_rlimit_cpu()
{
    printRlimit("RLIMIT_CPU Before:", RLIMIT_CPU);

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = cpu_limit_handler;
    if (sigaction(SIGXCPU, &sa, NULL) == -1)
        errExit("sigaction");

    sa.sa_handler = alarm_handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    alarm(5);
    while (1)   /* Burn 5 seconds of cpu time... */
        if (alarm_trig == 1)
            break;

    /* ... Then set the soft limit lower but the hard limit higher */
    struct rlimit rlim;
    rlim.rlim_cur = 2;
    rlim.rlim_max = 10;

    if (setrlimit(RLIMIT_CPU, &rlim) == -1)
        errExit("setrlimit");

    printRlimit("RLIMIT_CPU After:", RLIMIT_CPU);

    while (1)
        ;   /* Now just keep burning time */

    /*
    The output I received:

        RLIMIT_AS Before: soft=infinite; hard=infinite
        RLIMIT_AS After: soft=2; hard=10

        [sec: 1764087860, us: 91638] Caught SIGXCPU!
        [sec: 1764087860, us: 92625] Caught SIGXCPU!
        [sec: 1764087860, us: 93624] Caught SIGXCPU!
        [sec: 1764087860, us: 105627] Caught SIGXCPU!
        [sec: 1764087861, us: 108627] Caught SIGXCPU!
        [sec: 1764087862, us: 111626] Caught SIGXCPU!
        [sec: 1764087863, us: 115625] Caught SIGXCPU!
        [sec: 1764087864, us: 118626] Caught SIGXCPU!
        zsh: killed     ./limit_passed_cap.bin

    It seems as though I got a burst of SIGXCPU for each second over the
    soft limit I was once I set it. (Notice the seconds of the first three
    signals caught)

    Then I started recieving it once every second as described by the man page.

    Finally once the cpu time exceeded the hard limit, SIGKILL was sent which
    terminated the process.
    */
}

/************************* RLIMIT_FSIZE Test *************************/

#define FILE_SIZE 200
#define FILE_NAME "dummyfile.txt"

void file_size_handler(int sig)
{
    (void) sig;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    printf("[sec: %ld, us: %ld] Caught SIGXFSZ!\n", tv.tv_sec, tv.tv_usec);
}

/* Note* If you attempt a write larger then the FSIZE limit, write() will
 * not actually fail but succeed but only write up until the limit. So for
 * example if you have a FSIZE limit of 100b and attempt a 200b write,
 * write() will only write 100b (returning 100 per usual). */

void test_rlimit_fsize()
{
    printRlimit("RLIMIT_FSIZE Before:", RLIMIT_FSIZE);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = file_size_handler;
    if (sigaction(SIGXFSZ, &sa, NULL) == -1)
        errExit("sigaction");

    int fd = open(FILE_NAME, O_CREAT | O_EXCL | O_RDWR, 0700);
    if (fd == -1)
        errExit("open");

    if (unlink(FILE_NAME) == -1)   /* Unlink so that the file will not exist after */
        errExit("unlink");         /* the program inevitably crashes (Recall that
                                      an open fd in a program keeps the inode alive
                                      even with no file system links) */
    char buf[FILE_SIZE];
    memset(buf, 'a', FILE_SIZE);

    int numWritten;   /* Create a file and write some data to it... */
    if ((numWritten = write(fd, buf, FILE_SIZE)) == -1)
        errExit("write");

    struct rlimit rlim;
    rlim.rlim_cur = 100;
    rlim.rlim_max = 100;   /* ...Then set a limit smaller then the size of the file */
    if (setrlimit(RLIMIT_FSIZE, &rlim) == -1)
        errExit("setrlimit");

    printRlimit("RLIMIT_FSIZE After:", RLIMIT_FSIZE);

    /* Attempt to increase it even more */
    if (ftruncate(fd, 300) == -1)
        errExit("ftruncate");

    /*
    My output:

        RLIMIT_FSIZE Before: soft=infinite; hard=infinite
        RLIMIT_FSIZE After: soft=100; hard=100
        [sec: 1764091552, us: 880539] Caught SIGXFSZ!
        ftruncate: File too large
    */
}

/************************* RLIMIT_NICE Test *************************/

void test_rlimit_nice()
{
    printRlimit("RLIMIT_NICE Before:", RLIMIT_NICE);

    errno = 0;   /* nice() may legitimetely return -1 */

    int currNice;   /* Set nice value to 5... */
    if ((currNice = nice(5)) == -1 && errno != 0)
        errExit("nice");

    struct rlimit rlim;
    rlim.rlim_cur = 10;
    rlim.rlim_max = 10;   /* ...and the limit to 10 (as per 20 - rlim_cur) */
    if (setrlimit(RLIMIT_NICE, &rlim) == -1)
        errExit("setrlimit");

    /* For me the setrlimit immediately fails with operation not permitted so
     * I will assume that you cannot set a limit nicer then the current nice
     * value. */
}

/************************* RLIMIT_NOFILE Test *************************/

/* Fun fact! The default soft is 2048 and the default hard is 524288 */

void test_rlimit_nofile()
{
    printRlimit("RLIMIT_NOFILE Before:", RLIMIT_NOFILE);

    /* By default we already have 3 open file descriptors (terminal ones) so
       we will just set a limit lower then 3. */

    struct rlimit rlim;
    rlim.rlim_max = 2;
    rlim.rlim_cur = 2;
    if (setrlimit(RLIMIT_NOFILE, &rlim) == -1)
        errExit("setrlimit");

    printRlimit("RLIMIT_NOFILE After:", RLIMIT_NOFILE);

    /* Attempt to open another file */

    if (open("test.txt", O_CREAT | O_EXCL, 0700) == -1)
        errExit("test.txt");

    /* So this ones allows you to set a lower limit then what you currently
     * have open and just disallows further openings until you are sufficiently
     * under the new limit */
}

/************************* RLIMIT_NPROC Test *************************/

void test_rlimit_nproc()
{
    printRlimit("RLIMIT_NPROC Before:", RLIMIT_NPROC);

    /* Create 5 children ... */
    for (int i = 0; i < 6; ++i) {
        switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            printf("Child created!\n");
            exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    struct rlimit rlim;
    rlim.rlim_cur = 3;
    rlim.rlim_max = 3;   /* ... and set the limit to 3 */
    if (setrlimit(RLIMIT_NPROC, &rlim) == -1)
        errExit("setrlimit");

    printRlimit("RLIMIT_NPROC After:", RLIMIT_NPROC);

    /* Setting the limit went fine, now lets try to create another process */

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        exit(EXIT_SUCCESS);
    default:
        break;
    }

    /* This fails so that means that even if a process has exited, if it hasn't
     * been waited for it still counts towards the limit. */

    /* Lets wait for all our children then try again */

    while (1)
        waitpid((pid_t) -1, NULL, 0);

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child created!\n");
        exit(EXIT_SUCCESS);
    default:
        break;
    }

    /*
    My output:

        RLIMIT_NPROC Before: soft=127432; hard=127432
        Child created!
        Child created!
        Child created!
        Child created!
        Child created!
        RLIMIT_NPROC After: soft=3; hard=3
        fork: Resource temporarily unavailable
        Child created!
    */
}

/************************* RLIMIT_SIGPENDING Test *************************/

void sig_test(int sig, siginfo_t *si, void *ucontext)
{
    (void) sig;
    (void) ucontext;

    printf("Signal #%d\n", si->si_value.sival_int);
}

void test_rlimit_sigpending()
{
    printRlimit("RLIMIT_SIGPENDING Before:", RLIMIT_SIGPENDING);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sig_test;
    if (sigaction(SIGRTMIN + 3, &sa, NULL) == -1)
        errExit("sigaction");

    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGRTMIN + 3);
    if (sigprocmask(SIG_SETMASK, &block, NULL) == -1)
        errExit("sigprocmask");

    union sigval sv;
    for (int i = 0; i < 20; ++i) {
        sv.sival_int = i + 1;   /* Send 20 blocked signals... */
        if (sigqueue(getpid(), SIGRTMIN + 3, sv) == -1)
            errExit("sigqueue");
    }

    struct rlimit rlim;
    rlim.rlim_max = 10;
    rlim.rlim_cur = 10;   /* ... And set the limit to just 10 */
    if (setrlimit(RLIMIT_SIGPENDING, &rlim) == -1)
        errExit("rlim");

    printRlimit("RLIMIT_SIGPENDING After:", RLIMIT_SIGPENDING);

    /* That worked so lets try and queue another one. */

    sv.sival_int = 21;   /* (signal #21) */
    if (sigqueue(getpid(), SIGRTMIN + 3, sv) == -1)
        perror("sigqueue");

    /* And that failed as expected. Lets unblock the signal and see if all
     * 20 get delivered. */

    sigemptyset(&block);
    if (sigprocmask(SIG_SETMASK, &block, NULL) == -1)
        errExit("sigprocmask");

    /*
    My output:

        RLIMIT_SIGPENDING Before: soft=127432; hard=127432
        RLIMIT_SIGPENDING After: soft=10; hard=10
        sigqueue: Resource temporarily unavailable
        Signal #1
        Signal #2
        ...
        Signal #19
        Signal #20
    */
}

int main(void)
{
    /* Uncomment and build to try each test */

    // test_rlimit_as();
    // test_rlimit_cpu();
    // test_rlimit_fsize();
    // test_rlimit_nice();
    // test_rlimit_nofile();
    // test_rlimit_nproc();
    // test_rlimit_sigpending();

    exit(EXIT_SUCCESS);
}
