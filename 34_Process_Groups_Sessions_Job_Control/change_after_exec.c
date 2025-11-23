#define _GNU_SOURCE

#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

int main(void)
{
    pid_t childId;
    switch ((childId = fork())) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child: sleeping...(PGRP = %d)\n", getpgrp());
        sleep(2);
        printf("Child: awake! (PGID = %d)\n", getpgrp());
        printf("Child: calling exec... (PGID = %d)\n", getpgrp());
        execlp("sleep", "sleep", "5", (char *) NULL);
        errExit("execl");
    default:
        sleep(1);

        printf("Parent: changing child PGID...\n");
        setpgid(childId, 0);   /* Make child a leader of a new PGRP */
        printf("Parent: Success! Parent sleeping...\n");

        sleep(2);

        // /* Try to make child return to old PGRP after exec */
        if (setpgid(childId, getpgrp()) == -1) {
            fprintf(stderr, "Parent: Failed to change child PGRP\n");
            fprintf(stderr, "errno: %d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Should never happen! */
        waitpid(childId, NULL, 0);

        break;
    }

    exit(EXIT_SUCCESS);
}
