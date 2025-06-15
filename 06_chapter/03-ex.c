////////////////////////////////////////////////////////////////////////
// 6.10 - 6-3. Reimplementing setenv() and unsetenv()
//
//
// Limitations:
// - No handling for invalid environ entries such as empty string or no
//   '=' character.
// - No error return values.
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>

extern char** environ;

void setenv_clone(const char* name, const char* value, int overwrite)
{
    const char* existingName = getenv(name);
    if (existingName && overwrite == 0)
        return;

    int nameLength = strlen(name);
    int valueLength = strlen(value);

    char* buf = malloc(nameLength + valueLength + 1);
    strcpy(buf, name);
    strcpy(buf + nameLength, "=");
    strcpy(buf + nameLength + 1, value);

    putenv(buf);
}

void unsetenv_clone(const char* name)
{
    if (environ == NULL)
        return;

    while (1) {
        int index = 0;
        int itExists = 0;
        for (char** i = environ; *i != NULL; ++i) {
            char buf[100];

            for (int y = 0; (*i)[y] != '\0'; ++y) {
                if ((*i)[y] == '=') {
                    buf[y] = '\0';
                    break;
                }

                buf[y] = (*i)[y];
            }

            if (strcmp(buf, name) == 0) {
                itExists = 1;
                break;
            }

            ++index;
        }

        if (!itExists)
            break;

        for (char** i = environ + index; *i != NULL; ++i) {
            if (!*(environ + index + 1)) {
                *(environ + index) = NULL;
                break;
            }

            *(environ + index) = *(environ + index + 1);

            ++index;
        }
    }
}

int main(void)
{
    clearenv();

    char name[] = "test";
    char value[] = "variable";
    setenv_clone(name, value, 0);

    char middle[] = "middle";
    char middle_value[] = "variable";
    setenv_clone(middle, middle_value, 0);

    char name2[] = "test2";
    char value2[] = "varaible2";
    setenv_clone(name2, value2, 0);

    // Manually create duplicate to test duplicate removal
    environ[2] = "test=varaible";

    unsetenv_clone("test");

    for (char** i = environ; *i != NULL; ++i)
        puts(*i);
}
