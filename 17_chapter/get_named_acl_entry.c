
/*
Name:
    get_named_acl_entry - Print relevant acl entries of a file

Usage:
    get_named_acl_entry [ug] <UID/GID> file

    -u, The following id will be interpreted as a UID
    -g, The following id will be interpreted as a GID

Description:
    This program was created as a solution to TLPI exersise 17-1

    This program prints the acl entries associated with the uid or
    gid passed in as a command line argument. If the user owns the
    file and has a named entry the program will print both. If the
    entry is part of the group class (ACL_USER, ALC_GROUP and
    ACL_GROUP_OBJ) the effective permissions after masking will
    also be printed.

Notes:
    This program was NOT extensively tested although the behaviour
    was consistent during use.

    There is probably a better way to determine the masked values of
    the permset instead of looping through the acl object again.
    (getMaskPermset())
*/

#include <getopt.h>
#include <sys/types.h>
#include <sys/acl.h>
#include <sys/stat.h>
#include <acl/libacl.h>

#include "../lib/error_functions.h"

int getPermChar(acl_permset_t *permset, acl_permset_t *mask,
    int perm)
{
    int permVal = acl_get_perm(*permset, perm);
    if (permVal == -1)
        errExit("acl_get_perm");

    int maskVal = 1;   /* 1 if contains perm */
    if (mask) {
        maskVal = acl_get_perm(*mask, perm);
        if (maskVal == -1)
            errExit("acl_get_perm");
    }

    return permVal == 1 && maskVal == 1;
}

char* getPermString(acl_permset_t *permset, acl_permset_t *mask)
{
    char* permString = malloc(4);

    permString[0] = (getPermChar(permset, mask, ACL_READ)) ? 'r' : '-';
    permString[1] = getPermChar(permset, mask, ACL_WRITE) ? 'w' : '-';
    permString[2] = getPermChar(permset, mask, ACL_EXECUTE) ? 'x' : '-';
    permString[3] = '\0';

    return permString;
}
   /* Returns dynamically allocated mask permset if it exists */
acl_permset_t* getMaskPermset(const char* filePath)
{
    acl_t acl;
    acl_type_t entryId;
    acl_entry_t entry;
    acl_tag_t tag;
    acl_permset_t* permset = malloc(sizeof(acl_permset_t));
    int rv;

    acl = acl_get_file(filePath, ACL_TYPE_ACCESS);
    if (acl == NULL)
        errExit("acl_get_file");

    for (entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY) {

        if ((rv = acl_get_entry(acl, entryId, &entry)) != 1)
            break;   /* break on error or no more entries */

        if (acl_get_tag_type(entry, &tag))
            errExit("acl_get_tag_type");

        if (tag == ACL_MASK) {
            if (acl_get_permset(entry, permset) == -1)
                errExit("acl_get_permset");

            return permset;
        }
    }

    if (rv == -1)
        errExit("acl_get_entry");

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        usageErr("get_named_acl_entry.bin [ug] name file");

    uid_t uid = -1;
    gid_t gid = -1;

    int opt;
    char *endptr;
    while ((opt = getopt(argc, argv, "u:g:")) != -1) {
        switch (opt) {
        case 'u':
            uid = strtol(optarg, &endptr, 10);
            if (*endptr != '\0')
                errExit("Bad conversion");
            break;
        case 'g':
            gid = strtol(optarg, &endptr, 10);
            if (*endptr != '\0')
                errExit("strol");
            break;
        case '?':
            usageErr("get_named_acl_entry.bin [ug] name file");
            break;
        }
    }

    struct stat sb;
    if (stat(argv[optind], &sb) == -1)
        errExit("stat");

    int isFileOwner = 0;
    if (sb.st_uid == uid)
        isFileOwner = 1;

    int isGroupOwner = 0;
    if (sb.st_gid == gid)
        isGroupOwner = 1;

    int hasNamedEntry = 0;

    acl_t acl;
    acl_entry_t entry;
    acl_tag_t tag;
    acl_permset_t permset;
    void *tagQualifier;
    char* otherPermString;

    acl = acl_get_file(argv[optind], ACL_TYPE_ACCESS);
    if (acl == NULL)
        errExit("acl_get_file");

    for (int entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY) {

        if (acl_get_entry(acl, entryId, &entry) != 1)
            break;   /* break on error or no more entries */

        if (acl_get_tag_type(entry, &tag) == -1)
            errExit("acl_get_tag_type");

        if (tag == ACL_USER_OBJ && isFileOwner) {
            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            char *permString = getPermString(&permset, NULL);
            printf("ACL_USER_OBJ: %s\n", permString);
            free(permString);

        } else if (tag == ACL_USER) {
            tagQualifier = acl_get_qualifier(entry);
            if (tagQualifier == NULL)
                errExit("acl_get_qualifier");

            if (!(*(uid_t*)tagQualifier == uid)) {
                acl_free(tagQualifier); /* Must call free on qualifiers */
                continue;   /* continue if doesn't match passed in uid */
            }

            acl_free(tagQualifier);

            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            char *permString = getPermString(&permset, NULL);
            printf("ACL_USER: %s   ", permString);
            free(permString);

            acl_permset_t *maskPermset = getMaskPermset(argv[optind]);
            char *maskedString = getPermString(&permset, maskPermset);
            printf("effective: %s\n", maskedString);
            free(maskPermset);

            hasNamedEntry = 1;

        } else if (tag == ACL_GROUP_OBJ && isGroupOwner) {
            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            char *permString = getPermString(&permset, NULL);
            printf("ACL_GROUP_OBJ: %s   ", permString);
            free(permString);

            acl_permset_t *maskPermset = getMaskPermset(argv[optind]);
            char *maskedString = getPermString(&permset, maskPermset);
            printf("effective: %s\n", maskedString);
            free(maskPermset);

        } else if (tag == ACL_GROUP) {
            tagQualifier = acl_get_qualifier(entry);
            if (tagQualifier == NULL)
                errExit("acl_get_qualifier");

            if (!(*(gid_t*)tagQualifier == gid)) {
                acl_free(tagQualifier);
                continue;
            }

            acl_free(tagQualifier);

            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            char *permString = getPermString(&permset, NULL);
            printf("ACL_GROUP: %s   ", permString);
            free(permString);

            acl_permset_t *maskPermset = getMaskPermset(argv[optind]);
            char *maskedString = getPermString(&permset, maskPermset);
            printf("effective: %s\n", maskedString);
            free(maskPermset);

            hasNamedEntry = 1;

        } else if (tag == ACL_OTHER) {
            if (acl_get_permset(entry, &permset) == -1)
                errExit("acl_get_permset");

            otherPermString = getPermString(&permset, NULL);
        }
    }

    if (!isFileOwner && !isGroupOwner && !hasNamedEntry)
        printf("ACL_OTHER: %s\n", otherPermString);

    free(otherPermString);
    acl_free(acl);
    exit(EXIT_SUCCESS);
}
