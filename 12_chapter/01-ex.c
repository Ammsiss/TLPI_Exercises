////////////////////////////////////////////////////////////////////////
// 12.4 - 12-1. List proccess by user
////////////////////////////////////////////////////////////////////////

/*
 * TODO!
 *
 * Shell script that verifies output (uid matches passed in username in
 * all printed entries)
 *
 * Modularize dynamic growth of array
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <linux/sched.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "error_functions.h"

#define MAX_COMM_NAME 64
#define MAX_PID 32
#define MAX_PID_LENGTH 9

struct ProcessInfo {
    char pid[MAX_PID];
    char name[MAX_COMM_NAME];
};

uid_t           /* Return UID corresponding to 'name', or -1 on error */
userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0')  /* On NULL or empty string */
        return -1;                      /* return an error */

    u = strtol(name, &endptr, 10);      /* As a convenience to caller */
    if (*endptr == '\0')                /* allow a numeric string */
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

char* getline_clone(int fd)
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

size_t getCharIndex(const char *line, char target)
{
    size_t i = 0;
    for (const char *c = line; *c != '\0'; ++c) {
        if (*c == target)
            break;

        ++i;
    }

    return i;
}

// Return static buffer
char* getValue(const char *line)
{
    static char value[50];

    size_t i = 0;
    size_t startIndex = getCharIndex(line, '\t') + 1;
    for (const char *c = line + startIndex; *c != '\0' && *c != '\t'; ++c) {
        value[i] = *c;
        ++i;
    }

    value[i] = '\0';

    return value;
}

// Return static buffer
char* getField(const char *line)
{
    static char field[50];

    size_t i = 0;
    size_t firstDelim = getCharIndex(line, ':');
    while(i < firstDelim) {
        field[i] = line[i];
        ++i;
    }

    field[firstDelim] = '\0';

    return field;
}

int getProcessInfo(uid_t uid, struct ProcessInfo* info, const char *processID)
{
    char *path = malloc(4096);
    if (!path) errExit("malloc");
    path[0] = '\0';

    const char *header = "/proc/";
    const char *tail = "/status";

    strcat(path, header);
    strcat(path, processID);
    strcat(path, tail);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        if (errno == ENOENT) // If associated pid dir is gone, return
            return -1;
        else
            errExit("open");
    }

    free(path);

    const char *name;
    const char *pid;

    int collected = 0;

    while (1) {

        const char *line = getline_clone(fd);
        if (!line) errExit("getline_clone");

        const char *field = getField(line);
        if (!field) errExit("getField");

        if (strcmp(field, "Uid") == 0) {

            const char *value = getValue(line);
            if (!value) errExit("getValue");

            char *endptr;
            uid_t uidVal = strtol(value, &endptr, 10);
            if (*endptr != '\0') errExit("strtol");

            if (uid != uidVal) {
                free((void *)line);
                return -1;
            }

            ++collected;

        } else if (strcmp(field, "Name") == 0) {

            name = getValue(line);
            if (!name) errExit("getValue");

            strcpy(info->name, name);

            ++collected;

        } else if (strcmp(field, "Pid") == 0) {

            pid = getValue(line);
            if (!pid) errExit("getValue");

            strcpy(info->pid, pid);

            ++collected;
        }

        free((void *)line);

        if (collected == 3) {
            break;
        }
    }

    close(fd);
    return 0;
}

void print(const char *username, struct ProcessInfo *processList, size_t size)
{
    printf("\nThe user '%s' owns the following proccesses:\n\n", username);

    printf("    Pid           Name\n");
    printf("--------------------------\n");

    for (size_t i = 0; i < size; i++) {
        int pidLen = strlen(processList[i].pid);
        int spaces = MAX_PID_LENGTH - pidLen;

        char spacing[spaces + 1];
        memset(spacing, ' ', spaces);
        spacing[spaces] = '\0';

        printf("    %s%s|    %s\n", processList[i].pid, spacing, processList[i].name);
    }

    printf("--------------------------\n");
}

int main(int argc, char** argv)
{
    if (argc != 2)
        usageErr("upro <username>");

    uid_t uid = userIdFromName(argv[argc - 1]);
    if (uid == -1u)
        messExit("Invalid username.");

    size_t size = 64;
    struct ProcessInfo *processList = malloc(sizeof(struct ProcessInfo) * size);

    DIR *dirStream = opendir("/proc");
    size_t directoryCount = 0;

    struct dirent *entry;
    while ((entry = readdir(dirStream)) != NULL) {

        char *endPtr;
        strtol(entry->d_name, &endPtr, 10);
        if (*endPtr != '\0')
            continue;

        if (getProcessInfo(uid, &processList[directoryCount], entry->d_name) != -1)
            ++directoryCount;

        if (directoryCount + 1 >= size) {

            struct ProcessInfo *temp;
            temp = realloc(processList, sizeof(struct ProcessInfo) * size * 2);

            if (!temp) {
                free(processList);
                errExit("realloc");
            }

            size *= 2;
            processList = temp;
        }
    }

    closedir(dirStream);

    print(argv[argc- 1], processList, directoryCount);
    free(processList);

    exit(EXIT_SUCCESS);
}
