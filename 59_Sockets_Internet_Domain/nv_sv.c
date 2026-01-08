#include "tlpi_hdr.h" // IWYU pragma: export
#include "binary_tree.h"
#include "nv.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 50

int main(void)
{
    BTree tree;
    bt_initialize(&tree);

    BTree auth_tree;
    bt_initialize(&auth_tree);

    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = 50000;
    svaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1)
        errExit("bind");

    if (listen(sfd, BACKLOG) == -1)
        errExit("listen");

    struct request_msg req;

    int cfd;
    while (1) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            errMsg("Failed to accept connection");
            continue;
        }

        int num_read;
        if ((num_read = read(cfd, &req, REQ_SIZE)) != REQ_SIZE) {
            errMsg("Failed/Partial read; dropping request");
            close(cfd);
            continue;
        }

        char *lookup;
        char *lookup_pass;

        switch (req.command) {
        case CMD_ADD:
            char *value = malloc(MAX_VAL);
            if (!value)
                errExit("malloc");

            strcpy(value, req.value);

            if (bt_add(&tree, req.key, value) == -1) {
                errMsg("Duplicate key entered");
                free(value);
            }

            char *password = malloc(PAS_LEN);
            if (!password)
                errExit("malloc");

            strcpy(password, req.password);

            if (bt_add(&auth_tree, req.key, password) == -1) {
                errMsg("Couldn't add password");
                free(password);
            }

            break;
        case CMD_DEL:
            if (bt_lookup(&auth_tree, req.key, (void *) &lookup_pass) == -1) {
                errMsg("Key doesn't exist");
                break;
            }

            if (strcmp(lookup_pass, req.password) != 0) {
                errMsg("Couldn't authenticate");
                break;
            }

            if (bt_delete(&tree, req.key) == -1)
                errMsg("Key does not exist");
            break;
        case CMD_MOD:
            if (bt_lookup(&auth_tree, req.key, (void *) &lookup_pass) == -1) {
                errMsg("Key doesn't exist");
                break;
            }

            if (strcmp(lookup_pass, req.password) != 0) {
                errMsg("Couldn't authenticate");
                break;
            }

            if (bt_lookup(&tree, req.key, (void *) &lookup) == -1)
                errMsg("Key value pair does not exist");
            else {
                printf("%s->", lookup);
                strcpy(lookup, req.value);
                printf("%s\n", lookup);
            }
            break;
        case CMD_GET:
            if (bt_lookup(&tree, req.key, (void *) &lookup) == -1)
                errMsg("Key value pair does not exist");
            else
                printf("Value: %s\n", lookup);
            break;
        }

        bt_print(&tree);
        close(cfd);
    }

    bt_free(&tree);
    bt_free(&auth_tree);
}
