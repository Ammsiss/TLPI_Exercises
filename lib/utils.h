#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstddef>
#include <cstdlib>

static char* fgetstr(int fd)
{
    size_t size = 64;
    size_t totalRead = 0;

    char character;
    char *line = malloc(size);
    if (!line)
        return NULL;

    while (1) {
        if (totalRead + 1 >= size) {
            char *temp = realloc(line, size * 2);
            if (temp == NULL) {
                free(line);
                return NULL;
            }

            size *= 2;
            line = temp;
        }

        int rv = read(fd, &character, 1);
        if (rv == -1) {
            free(line);
            return NULL;
        }

        if (rv == 0 || character == '\n')
            break;

        line[totalRead++] = character;
    }

    line[totalRead] = '\0';

    return line;
}
