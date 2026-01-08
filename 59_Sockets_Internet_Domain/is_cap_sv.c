#include "is_cap.h"

int main(void)
{
    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY;
    svaddr.sin_port = htons(PORT_NUM);

    if (bind(sfd, (struct sockaddr*) &svaddr, sizeof(struct sockaddr_in)) == -1)
        errExit("bind");

    if (listen(sfd, 10) == -1)
        errExit("listen");

    int con_sfd;
    if ((con_sfd = accept(sfd, NULL, NULL)) == -1)
        errExit("accept");

    char msg[BUF_SIZE];

    if (read(con_sfd, &msg, BUF_SIZE) == -1)
        errExit("Failed/Partial read");

    int msg_len = strlen(msg) + 1;

    for (int i = 0; i < msg_len - 1; ++i)
        msg[i] = toupper(msg[i]);

    if (write(con_sfd, &msg, msg_len) != msg_len)
        errExit("Failed/Partial write");

    if (close(con_sfd) == -1)
        errExit("close");
    if (close(sfd) == -1)
        errExit("close");

    return EXIT_SUCCESS;
}
