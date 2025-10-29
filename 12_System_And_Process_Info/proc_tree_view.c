/*
 * Write a program that draws a tree showing the hierarchical parent-child
 * relationships of all processes on the system, going all the way back to init.
 *
 * For each process, the program should display the process ID and the command being
 * executed. /proc/<pid>/cmdline
 *
 * The output of the program should be similar to that produced by pstree(1),
 * although it does need not to be as sophisticated.
 *
 * The parent of each process on the system can be found by inspecting the PPid: line
 * of all of the /proc/PID/status files on the system.
 *
 * Be careful to handle the possibility that a process’s parent (and thus its /proc/PID
 * directory) disappears during the scan of all /proc/PID directories.
 */

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "error_functions.h"
#include "dynamic_array.h"
#include "utils.h"

#define ID_MAX 32

struct ProcessInfo
{
    char pid[ID_MAX];
    char ppid[ID_MAX];
    char *command;
    bool taken;
    bool parent;
};

void cleanup(DynArray *dirList, DynArray *indentLevels)
{
    for (size_t i = 0; i < dirList->size; ++i)
        free(((struct ProcessInfo *)da_get(dirList, i))->command);

    da_free(dirList);
    da_free(indentLevels);
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
    static char value[ID_MAX];

    size_t i = 0;
    size_t startIndex = getCharIndex(line, '\t') + 1;
    for (const char *c = line + startIndex; *c != '\n' && *c != '\t' && *c != '\0'; ++c) {
        value[i] = *c;
        ++i;
    }

    value[i] = '\0';

    return value;
}

// Return static buffer
char* getField(const char *line)
{
    static char field[ID_MAX];

    size_t i = 0;
    size_t delim = getCharIndex(line, ':');
    while(i < delim) {
        field[i] = line[i];
        ++i;
    }

    field[delim] = '\0';

    return field;
}

// return static buffer
static const char* getID(const char *processID, const char *field) {

    char *path = malloc(4096);
    if (path == NULL) errExit("malloc");
    path[0] = '\0';

    const char *header = "/proc/";
    const char *tail = "/status";

    strcat(path, header);
    strcat(path, processID);
    strcat(path, tail);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        free(path);
        errExit("open");
    }

    free(path);

    // Assumes field exists
    while (1) {
        char *line;
        fgetln(fd, &line);

        if (!line)
            errExit("fgetln");

        const char *f = getField(line);
        if (!f) {
            free(line);
            errExit("getField");
        }

        if (strcmp(field, f) == 0) {
            const char *value;
            value = getValue(line);
            free(line);

            return value;
        }

        free(line);
    }
}


// return dyn memory in format "<cmd>\0"
static char* getCommand(const char *processID)
{
    char *path = malloc(PATH_MAX);
    if (path == NULL) errExit("malloc");
    path[0] = '\0';

    const char *header = "/proc/";
    const char *tail = "/cmdline";

    strcat(path, header);
    strcat(path, processID);
    strcat(path, tail);

    int fd = open(path, O_RDONLY);
    if (fd == -1) errExit("open");

    free(path);

    // Get command
    char *cmd;
    int cmdSize = fgetln(fd, &cmd);

    if (cmd == NULL) errExit("fgetln");

    close(fd);

    for (int i = 0; i < cmdSize - 1; ++i)
        if (cmd[i] == '\0')
            cmd[i] = ' ';

    return cmd;
}

bool noMoreProcesses()
{
    return false;
}

bool hasChildren(const char *pid, const DynArray *dirList) {
    const struct ProcessInfo *entry;
    for (size_t i = 0; i < dirList->size; ++i) {
        entry = da_get(dirList, i);
        if (strcmp(entry->ppid, pid) == 0 && !entry->taken)
            return true;
    }

    return false;
}

struct ProcessInfo* findChild(const char *pid, const DynArray *dirList) {
    struct ProcessInfo *entry;
    for (size_t i = 0; i < dirList->size; ++i) {
        entry = da_get(dirList, i);
        if (strcmp(entry->ppid, pid) == 0 && !entry->taken) {
            entry->taken = true;
            return entry;
        }
    }

    // No more parents left
    return NULL;
}

char* getSpacing(int size)
{
    if (size < 0)
        size = 0;

    char *spacing = malloc(size + 1);
    memset(spacing, ' ', size);
    spacing[size] = '\0';

    return spacing;
}

DynArray indentLevels;

// Why can't shallow copy pointer?
void printHierarchy(const DynArray *dirList, const struct ProcessInfo *parent,
    int initSpacing, bool firstChild, bool lastChild, bool multiChild)
{
    if (multiChild)
        *(int *)da_push(&indentLevels) = initSpacing;

    int parentLength = initSpacing;
    char *parentSpacing = getSpacing(parentLength - 3);

    int childLength = initSpacing + 3 + strlen(parent->pid);
    char *childSpacing = getSpacing(childLength - 3);

    for (size_t i = 0; i < indentLevels.size; ++i) {
        if (*(int *)da_get(&indentLevels, i) > 2)
            childSpacing[*(int *)da_get(&indentLevels, i) - 2] = '|';
    }

    // PROCESS //////////////////////////////////////////////////////////////
    DynArray children;
    da_init(&children, 0, sizeof(struct ProcessInfo *));

    while (1) {
        struct ProcessInfo *child = findChild(parent->pid, dirList);
        if (child == NULL)
            break;

        if (hasChildren(child->pid, dirList)) {
                child->parent = true;
                *(struct ProcessInfo **)da_push(&children) = child;
        } else {
            *(struct ProcessInfo **)da_push(&children) = child;
        }
    }

    // PRINT PARENT //////////////////////////////////////////////////////////
    const char *headerFirstSingle = "---";
    const char *headerFirstMulti = "-+-";
    const char *headerMiddle = " |-";
    const char *headerLast = " `-";
    // const char *headerFirstSingle = "───";
    // const char *headerFirstMulti = "─┬─";
    // const char *headerMiddle = " ├─";
    // const char *headerLast = " └─";

    if (!firstChild) {
        if (lastChild)
            printf("%s", headerLast);
        else
            printf("%s", headerMiddle);
    }

    if (children.size > 1)
        printf("%s%s", parent->pid, headerFirstMulti);
    else
        printf("%s%s", parent->pid, headerFirstSingle);

    // PRINT CHILD ////////////////////////////////////////////////////////////
    bool first = true;
    if (children.size > 0) {
        for (size_t i = 0; i < children.size; ++i) {
            struct ProcessInfo **child = da_get(&children, i);

            if ((*child)->parent) {

                if (!first)
                    printf("%s", childSpacing);

                printHierarchy(dirList, *child, childLength, first, i == children.size - 1,
                    children.size > 1);

                if (children.size > 1)
                    da_pop(&indentLevels);
            } else {
                if (!first) {
                    if (i == 0) {
                        if (children.size > 1)
                            printf("%s%s", childSpacing, headerFirstMulti);
                        else if (children.size == 1)
                            printf("%s%s", childSpacing, headerFirstSingle);
                    } else if (children.size - 1 != i)
                        printf("%s%s", childSpacing, headerMiddle);
                    else
                        printf("%s%s", childSpacing, headerLast);
                }
                printf("%s\n", (*child)->pid);
            }

            first = false;
        }
    }

    free(childSpacing);
    free(parentSpacing);
    da_free(&children);
}

int main()
{
    DynArray dirList;
    da_init(&dirList, 0, sizeof(struct ProcessInfo));

    DIR *dirStream = opendir("/proc");

    // Extract relevent data
    struct dirent *entry;
    while ((entry = readdir(dirStream)) != NULL) {

        // if not a numbered directory, skip
        char *endptr;
        strtol(entry->d_name, &endptr, 10);
        if (*endptr != '\0') continue;

        struct ProcessInfo *info = (struct ProcessInfo *)da_push(&dirList);
        if (info == NULL) errExit("da_push");

        strcpy(info->pid, entry->d_name);                 // store pid
        strcpy(info->ppid, getID(entry->d_name, "PPid")); // store ppid
        char *cmd = getCommand(entry->d_name);            // store command
        info->command = cmd;

    } closedir(dirStream);

    struct ProcessInfo *initProcess;
    for (size_t i = 0; i < dirList.size; ++i) {
        struct ProcessInfo *element = da_get(&dirList, i);
        if (element == NULL)
            errExit("da_get");

        if (strcmp(element->pid, "1") == 0) {
            initProcess = element;
        }
    }

    da_init(&indentLevels, 0, sizeof(int));
    printHierarchy(&dirList, initProcess, 0, true, true, true);
    cleanup(&dirList, &indentLevels);
}
