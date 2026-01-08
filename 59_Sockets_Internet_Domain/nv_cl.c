#include "tlpi_hdr.h" // IWYU pragma: export
#include "nv.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void printHelp() {
    printf("COMMANDS:\n"
        "    d key       - delete a key value pair\n"
        "    a key value - add a key value pair\n"
        "    m key value - modify a key value pair\n"
        "    g key       - get a value associated with a key\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printHelp();
    }

    if (argc < 3)
        usageErr("%s command key value", argv[0]);

    struct request_msg req;

    printf("Enter password: ");
    fflush(stdin);

    fgets(req.password, 30, stdin);
    if (req.password[strlen(req.password) - 1] == '\n')
        req.password[strlen(req.password) - 1] = '\0';

    switch (argv[1][0]) {
    case 'd': req.command = CMD_DEL; break;
    case 'a': req.command = CMD_ADD; break;
    case 'm': req.command = CMD_MOD; break;
    case 'g': req.command = CMD_GET; break;
    default: usageErr("%s command key value", argv[0]);
    }

    if (strlen(argv[2]) >= MAX_KEY)
        cmdLineErr("Key len must be less then %d", MAX_KEY);
    if (argc == 4 && strlen(argv[3]) >= MAX_VAL)
        cmdLineErr("Value len must be less then %d", MAX_VAL);

    strcpy(req.key, argv[2]);

    if (argc == 4)
        strcpy(req.value, argv[3]);

    int cfd;
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = 50000;
    if (inet_aton("127.0.0.1", &svaddr.sin_addr) == 0)
        fatal("inet_aton input string invalid");

    if (connect(cfd, (struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_in)) == -1)
        errExit("connect");

    if (write(cfd, &req, REQ_SIZE) != REQ_SIZE)
        fatal("Failed/Partial write");

    close(cfd);
}
