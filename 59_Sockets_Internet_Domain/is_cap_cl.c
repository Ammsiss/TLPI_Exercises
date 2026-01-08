#include "is_cap.h"

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg", argv[0]);
    if (strlen(argv[1]) > BUF_SIZE - 1)
        cmdLineErr("msg can only be up to 20 in length");

    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY;
    svaddr.sin_port = htons(PORT_NUM);

    if (connect(sfd, (struct sockaddr*) &svaddr,
                sizeof(struct sockaddr_in)) == -1)
        errExit("connect");

    char msg[BUF_SIZE];
    strcpy(msg, argv[1]);

    int msg_len = strlen(msg) + 1;

    if (write(sfd, &msg, msg_len) != msg_len)
        errExit("Failed/Partial write");
    if (read(sfd, &msg, BUF_SIZE) == -1)
        errExit("Failed read");

    printf("%s\n", msg);

    if (close(sfd) == -1)
        errExit("close");

    return EXIT_SUCCESS;
}
