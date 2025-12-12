#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

#include "../lib/binary_tree.h"
#include "../lib/error_functions.h"

char *create_key(char *str)
{
    if (strlen(str) > BT_KEY_MAX) {
        errno = EINVAL;
        return NULL;
    }

    char *key = malloc(BT_KEY_MAX + 1);
    if (key == NULL)
        return NULL;

    strcpy(key, str);

    return key;
}


void handle_op(BTree *tree, char *command, int *quit, int *print_tree)
{
    switch (command[0]) {
    case 'd':
        if (bt_delete(tree, &command[1]) == -1) {
            printf("Bad key! Enter 'h' for help\n");
            *print_tree = 0;
        }
        break;
    case 'a':
        if (bt_add(tree, &command[1], NULL) == -1) {
            printf("Duplicate key entered!\n");
            *print_tree = 0;
        }
        break;
    case 'q':
        *quit = 1;
        break;
    case 'p':
        break;
    case 'h':
        printf("p - Print tree\nq - Quit\nd<key> - delete a node\n");
        *print_tree = 0;
        break;
    default:
        printf("Invalid command type! (Enter 'h' for help)\n");
        *print_tree = 0;
        break;
    }
}

int main(void)
{
    BTree tree;
    bt_initialize(&tree);

    for (int i = 0; i < 50; i++)
        printf("\n");
    printf("--------------------\n");
    bt_print(&tree);

    char *line = NULL;
    size_t len = 0;
    int quit = 0;
    int print_tree;

    while (1) {

        print_tree = 1;

        printf("--------------------\n");
        printf("Enter operation: ");
        fflush(stdout);

        ssize_t num_read = getline(&line, &len, stdin);

        if (feof(stdin)) {   /* If EOF is set */
            free(line);
            break;
        } else if (num_read == -1) {
            free(line);
            errExit("getline");
        }
        line[num_read - 1] = '\0';

        switch (strlen(line)) {
        case 0:
            printf("Invalid input! (Enter 'h' for help)\n");
            print_tree = 0;
            break;
        default:
            handle_op(&tree, line, &quit, &print_tree);
            break;
        }

        free(line);
        line = NULL;

        if (quit)
            break;

        if (print_tree) {
            for (int i = 0; i < 50; i++)
                printf("\n");
            printf("--------------------\n");
            bt_print(&tree);
        }
    }

    bt_free(&tree);

    exit(EXIT_SUCCESS);
}
