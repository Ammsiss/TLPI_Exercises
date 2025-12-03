#define _GNU_SOURCE

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>

#include "../lib/error_functions.h"

static int init = 1;
static size_t saved_fd_max;
static FILE **fd_to_stream;

void free_structure(void)
{
    free(fd_to_stream);
}

FILE *popen_imp(const char *command, const char *type)
{
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == -1)
        return NULL;

    if (init) {
        if (atexit(free_structure) != 0)
            return NULL;
        fd_to_stream = malloc(rlim.rlim_cur * sizeof(FILE *));
        saved_fd_max = rlim.rlim_cur;
        init = 0;
    } else if (saved_fd_max < rlim.rlim_cur) {
        FILE **temp;
        temp = realloc(fd_to_stream, rlim.rlim_cur * sizeof(FILE *));
        if (temp == NULL)
            return NULL;
        fd_to_stream = temp;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        return NULL;

    int std_fd;
    int shell_fd;
    FILE *out;

    if (strcmp(type, "r") == 0) {
        out = fdopen(pipe_fd[0], "r");
        shell_fd = pipe_fd[1];
        std_fd = STDOUT_FILENO;
    } else if (strcmp(type, "w") == 0) {
        out = fdopen(pipe_fd[1], "w");
        shell_fd = pipe_fd[0];
        std_fd = STDIN_FILENO;
    } else {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return NULL;
    }

    switch (fork()) {
    case -1:
        return NULL;
    case 0:
    {
        fclose(out);

        if (std_fd != shell_fd) {
            if (dup2(shell_fd, std_fd) == -1)
                _exit(127);   /* stay consistent with system() behaviour */
            if (close(shell_fd) == -1)
                _exit(127);
        }

        execlp("sh", "sh", "-c", command, (char *) NULL);
        _exit(127);
    }
    default:
        break;
    }

    if (close(shell_fd) == -1)
        return NULL;

    fd_to_stream[fileno(out)] = out;
    return out;
}

int pclose_imp(FILE *stream)
{
    int fd = fileno(stream);
    if (fd == -1 )
        return -1;

    int status;
    if (waitpid(fd, &status, 0) == -1)
        return -1;

    if (fclose(fd_to_stream[fd]) == EOF)
        return -1;

    return status;
}

int main(void)
{
    /* Test reading */
    FILE *pipe_r;
    if ((pipe_r = popen_imp("ls -l", "r")) == NULL)
        errExit("popen_imp");

    char output[1000];

    int num_read;
    if ((num_read = read(fileno(pipe_r), output, 1000)) == -1)
        errExit("read - main");
    output[num_read] = '\0';

    printf("%s", output);

    /* Test writing */
    FILE *pipe_w;
    if ((pipe_w = popen_imp("tee", "w")) == NULL)
        errExit("popen_imp");

    int num_written;
    char message[100];
    while (1) {
        if (fgets(message, 5, stdin) == NULL)
            break;

        if ((unsigned long) (num_written = write(fileno(pipe_w), message, strlen(message)))
                != strlen(message))
            errExit("Partial write to pipe_w");
        if (num_written == -1)
            errExit("write");
    }

    int status;
    if ((status = pclose(pipe_r)) == -1)
        errExit("pclose");
    if (WIFEXITED(status))
        printf("Exit status of shell executing ls:  %d\n", WEXITSTATUS(status));

    if ((status = pclose(pipe_w)) == -1)
        errExit("pclose");
    if (WIFEXITED(status))
        printf("Exit status of shell executing tee: %d\n", WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}
