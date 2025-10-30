/*
 * Function for verifying structure of tree
 *
 * This test program should create a bunch of threads that all operate on the
 * same global tree while the main thread coordinates them and verifies the ordering
 * of the tree and validity of it from time to time.
 *
 * Perhaps the first 50 threads, create 50 nodes each. when they are done validate
 * node count and ordering. The next 50 threads delete 50 nodes each, validate empty
 * tree.
 *
 * Then do a mixed tests of inserts and deletes.
 *
 * Start with one big lock.
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "../lib/error_functions.h"

#include "ts_btree.h"

// #define THREAD_COUNT 1
// #define KEY_PER_THRD 1000000

// #define THREAD_COUNT 2
// #define KEY_PER_THRD 500000

// #define THREAD_COUNT 3
// #define KEY_PER_THRD 333333

#define THREAD_COUNT 100
#define KEY_PER_THRD 100

static BTree_ts *tree;

static char *create_key(struct random_data *buf)
{
    char *key = malloc(BT_KEY_MAX + 1);
    if (key == NULL)
        return NULL;

    char c;
    for (int i = 0; i < 10; ++i) {

        int32_t r_num;
        if (random_r(buf, &r_num) == -1)
            errExit("random_r");

        c = r_num % 94 + 33;
        key[i] = c;
    }
    key[10] = '\0';

    return key;
}

static void *start(void *arg)
{
    (void) arg;

    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");
    long seed = tv.tv_usec ^ tv.tv_sec ^ pthread_self();

    struct random_data buf;
    buf.state = NULL;
    char state[256];

    if (initstate_r(seed, state, sizeof(state), &buf) == -1)
        errExit("initstate_r");

    char **keys;
    keys = malloc(KEY_PER_THRD * sizeof(char *));
    if (keys == NULL)
        errExit("malloc");

    char *key;
    for (int i = 0; i < KEY_PER_THRD; ++i) {
        key = create_key(&buf);

        if (bt_add_ts(tree, key, NULL) == -1) {
            --i;
            continue;
        }

        keys[i] = key;
    }

    // for (int i = 0; i < KEY_PER_THRD; ++i) {
    //     void *value;
    //     if (bt_lookup_ts(tree, keys[i], &value) == -1)
    //         errExit("bt_delete_ts");
    // }

    for (int i = 0; i < KEY_PER_THRD; ++i)
        free(keys[i]);

    free(keys);

    return NULL;
}

int main(void)
{
    tree = malloc(sizeof(BTree_ts));
    if (tree == NULL)
        errExit("malloc");
    if (bt_initialize_ts(tree) == -1)
        errExit("bt_initialize_ts");

    int s;
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i)
        if ((s = pthread_create(&threads[i], NULL, &start, NULL)) != 0)
            errExitEN(s, "pthread_create");

    for (int i = 0; i < THREAD_COUNT; ++i)
        if ((s = pthread_join(threads[i], NULL)) != 0)
            errExitEN(s, "pthread_join");

    bt_print_ts(tree);

    if (bt_free_ts(tree) == -1)
        errExit("bt_free_ts");
    free(tree);
    exit(EXIT_SUCCESS);
}
