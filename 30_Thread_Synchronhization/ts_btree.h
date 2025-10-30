#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BT_KEY_MAX 255

#define ROOT 0
#define LEFT_TRAVERSAL 1
#define RIGHT_TRAVERSAL 2

#define LOCK(mtx, s, rv) \
    do { \
        if ((s = pthread_mutex_lock(mtx)) != 0) \
            return rv; \
    } while (0)

#define UNLOCK(mtx, s, rv) \
    do { \
        if ((s = pthread_mutex_unlock(mtx)) != 0) \
            return rv; \
    } while (0)

typedef struct BTNode_ts BTNode_ts;

struct BTNode_ts {
    pthread_mutex_t *mtx;
    BTNode_ts *left;
    BTNode_ts *right;
    char *key;
    void *value;
};

typedef struct {
    BTNode_ts **node;
    pthread_mutex_t *par_mtx;
} NodeMutex;

typedef struct {
    BTNode_ts *root;
    pthread_mutex_t *mtx;
} BTree_ts;

// DONE
int bt_initialize_ts(BTree_ts *tree)
{
    tree->root = NULL;

    tree->mtx = malloc(sizeof(pthread_mutex_t));
    if (tree->mtx == NULL)
        return -1;
    pthread_mutex_init(tree->mtx, NULL);

    return 0;
}

// DONE
int bt_create_node_ts(BTNode_ts **node, char *key, void *value)
{
    *node = malloc(sizeof(BTNode_ts));

    (*node)->mtx = malloc(sizeof(pthread_mutex_t));
    if ((*node)->mtx == NULL)
        return -1;
    pthread_mutex_init((*node)->mtx, NULL);

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->value = value;

    (*node)->key = malloc(BT_KEY_MAX + 1);
    if ((*node)->key == NULL)
        return -1;
    strcpy((*node)->key, key);

    return 0;
}

// DONE
int bt_add_rec_ts(BTNode_ts *node, pthread_mutex_t *mtx, char *key, void *value)
{
    int rv;
    int s;

    LOCK(node->mtx, s, -1);
    UNLOCK(mtx, s, -1);

    int strcmp_rv = strcmp(key, node->key);

    if (strcmp_rv == 0) {
        errno = EINVAL;
        rv = -1;
    } else {
        BTNode_ts **nextDirection;
        if (strcmp_rv < 0)
            nextDirection = &node->left;
        else
            nextDirection = &node->right;

        if (*nextDirection == NULL) {
            rv = bt_create_node_ts(nextDirection, key, value);
            UNLOCK(node->mtx, s, -1);
        } else {
            rv = bt_add_rec_ts(*nextDirection, node->mtx, key, value);
        }
    }

    return rv;
}

// DONE
int bt_add_ts(BTree_ts *tree, char *key, void *value)
{
    int rv;
    int s;

    LOCK(tree->mtx, s, -1);

    if (tree->root == NULL) {
        rv = bt_create_node_ts(&tree->root, key, value);
        UNLOCK(tree->mtx, s, -1);
    }
    else
        rv = bt_add_rec_ts(tree->root, tree->mtx, key, value);

    return rv;
}

// DONE
NodeMutex *bt_find_node_ts(BTNode_ts **node, pthread_mutex_t *mtx, char *key)
{
    int s;

    LOCK((*node)->mtx, s, NULL);
    UNLOCK(mtx, s, NULL);

    BTNode_ts **nextDirection;

    int rv = strcmp(key, (*node)->key);

    if (rv < 0)
        nextDirection = &(*node)->left;
    else if (rv > 0)
        nextDirection = &(*node)->right;
    else {
        NodeMutex *out = malloc(sizeof(NodeMutex));
        if (out == NULL)
            return NULL;
        out->node = node;
        out->par_mtx = mtx;
        return out;
    }

    if (*nextDirection == NULL) {
        UNLOCK((*node)->mtx, s, NULL);
        return NULL;
    }

    return bt_find_node_ts(nextDirection, (*node)->mtx, key);
}

// DONE
int bt_lookup_ts(BTree_ts *tree, char *key, void **value)
{
    int rv = 0;
    int s;

    LOCK(tree->mtx, s, -1);

    if (tree->root == NULL) {
        UNLOCK(tree->mtx, s, -1);
        rv = -1;
    } else {
        NodeMutex *target = bt_find_node_ts(&tree->root, tree->mtx, key);
        if (target == NULL)
            rv = -1;
        else
            *value = (*target->node)->value;
        UNLOCK((*target->node)->mtx, s, -1);
        free(target);
    }

    return rv;
}

void bt_free_node_ts(BTNode_ts **node)
{
    pthread_mutex_destroy((*node)->mtx);
    free((*node)->mtx);
    free((*node)->key);
    free(*node);
    *node = NULL;
}

/* -------------------------------------------------------------- */

int bt_delete_ts(BTree_ts *tree, char *target_key)
{
    int s;

    LOCK(tree->mtx, s, -1);

    if (tree->root == NULL) {
        UNLOCK(tree->mtx, s, -1);
        return -1;
    }

    NodeMutex *target = bt_find_node_ts(&tree->root, tree->mtx, target_key);
    if (target == NULL) {
        errno = EINVAL;
        return -1;
    }

    BTNode_ts *target_left = (*target->node)->left;
    BTNode_ts *target_right = (*target->node)->right;

    LOCK(target_left->mtx, s, -1);
    LOCK(target_right->mtx, s, -1);

    int num_children = 0;
    if (target_left != NULL)
        ++num_children;
    if (target_right != NULL)
        ++num_children;

    switch (num_children) {
    case 0:
        LOCK(target->par_mtx, s, -1);
        bt_free_node_ts(target->node);
        UNLOCK(target->par_mtx, s, -1);
        break;
    case 1:
        LOCK(target->par_mtx, s, -1);
        bt_free_node_ts(target->node);

        if (target_left)
            *(target->node) = target_left;
        else
            *(target->node) = target_right;
        UNLOCK(target->par_mtx, s, -1);
        break;
    case 2:
    {
        // BTNode_ts **delete_node = &(*target)->right;
        // while (1) {
        //     if ((*delete_node)->left == NULL)
        //         break;
        //     delete_node = &(*delete_node)->left;
        // }
        //
        // (*target)->value = (*delete_node)->value;
        // strcpy((*target)->key, (*delete_node)->key);
        //
        // BTNode_ts *delete_node_child = (*delete_node)->right;
        //
        // bt_free_node_ts(delete_node);
        //
        // *delete_node = delete_node_child;
        //
        // break;
    }
    }

    UNLOCK(target_left->mtx, s, -1);
    UNLOCK(target_right->mtx, s, -1);
    free(target);

    return 0;
}

/* -------------------------------------------------------------- */

// DONE
void bt_free_rec_ts(BTNode_ts **node)
{
    if ((*node)->left != NULL)
        bt_free_rec_ts(&(*node)->left);

    if ((*node)->right != NULL)
        bt_free_rec_ts(&(*node)->right);

    bt_free_node_ts(node);
}

// DONE (INHERENTLY SINGLE THREADED)
int bt_free_ts(BTree_ts *tree)
{
    int s;

    if (tree->root != NULL)
        bt_free_rec_ts(&tree->root);

    if ((s = pthread_mutex_destroy(tree->mtx)) != 0)
        return -1;
    free(tree->mtx);

    return 0;
}

// DONE
void bt_print_rec_ts(BTNode_ts *node, int traversal)
{
    static int space_count = 0;

    // Printing

    if (traversal == ROOT)
        printf("['%s'] ", node->key);
    else if (traversal == LEFT_TRAVERSAL) {

        char *spaces = malloc(space_count + 1);
        if (spaces == NULL) {
            perror("malloc");
            return;
        }
        memset(spaces, ' ', space_count);
        spaces[space_count] = '\0';

        printf("\n%s['%s'] ", spaces, node->key);

        free(spaces);
    }
    else if (traversal == RIGHT_TRAVERSAL)
        printf("['%s'] ", node->key);

    // Traveling

    if (node->right != NULL) {

        char buf[1000];
        sprintf(buf, "['%s'] ", node->key);
        space_count += strlen(buf);

        bt_print_rec_ts(node->right, RIGHT_TRAVERSAL);

        space_count -= strlen(buf);
    }

    if (node->left != NULL)
        bt_print_rec_ts(node->left, LEFT_TRAVERSAL);
}

// DONE
void bt_print_ts(BTree_ts *tree)
{
    int s;

    if ((s = pthread_mutex_lock(tree->mtx)) != 0) {
        printf("pthread_mutex_lock: %s\n", strerror(s));
        return;
    }

    if (tree->root != NULL) {
        bt_print_rec_ts(tree->root, ROOT);
        printf("\n");
    } else
        printf("Tree is empty.\n");

    if ((s = pthread_mutex_unlock(tree->mtx)) != 0) {
        printf("pthread_mutex_unlock: %s\n", strerror(s));
        return;
    }
}

/*
Parent-before-child, always. Every search/insert/delete acquires the current node’s lock, then the next node’s lock, then releases the current one. No exceptions. That way, your parent lock really is a choke point—nobody can slip into the child from above without obeying the order (and thus blocking behind you).
*/
/*
Yup — a single big mutex turns your tree into a very polite queue. If you want actual parallelism, you need to let threads work on disjoint regions without stepping on each other. Here are the main patterns, from “simple but decent” to “wizardry,” with the gotchas that matter.

Choose a concurrency model

1) Top-down lock coupling (a.k.a. “crabbing”).
Give each node its own lock. While searching/inserting/deleting, you lock the current node, lock the child you’re about to descend into, then unlock the parent. Keep a strict top-down order to avoid deadlock.

Pros: Conceptually simple; good for search paths that don’t overlap much.

Cons: Rotations (in AVL/Red-Black trees) require locking a small neighborhood (2–3 nodes), which complicates but is manageable if you keep a fixed locking order (e.g., always lock left-before-right, parent-before-child).

Use when: You want straightforward correctness and reasonable throughput without going full academic.

2) Per-node read–write locks.
If reads dominate, node-level RW-locks let many searches proceed concurrently; writers still use lock coupling but acquire write locks only where needed.

Pros: Big win when workloads are read-heavy.

Cons: RW-lock overhead and writer starvation if you’re not careful; rebalancing can temporarily block a larger region.

3) Optimistic traversal with validation.
Walk the tree without locking, but keep track of “version numbers” (or “epoch stamps”) stored in nodes. Before modifying, re-traverse the affected path and validate that none of the nodes’ versions changed; if they did, retry.

Pros: Fast reads; avoids taking locks unless necessary.

Cons: You must implement robust validation and handle retries. Rotations need careful multi-node validation. Works best when contention is low to moderate.

4) Structural-copy updates (RCU/COW flavor).
For insert/delete, build the new path (or small subtree) off to the side and swing one pointer at the root of the changed region (with a fence). Readers run lock-free against an immutable snapshot; memory reclamation uses epochs/RCU.

Pros: Blazing-fast readers, simple traversal logic.

Cons: More memory churn; you need safe reclamation (epochs, hazard pointers). Trickier for frequent writes.

5) Lock-free BSTs.
Harris–Michael style or later refinements use CAS (compare-and-swap) to link/unlink nodes and mark edges for deletion.

Pros: High scalability under contention; no locks.

Cons: Considerably complex; you must solve ABA and memory reclamation correctly (hazard pointers, epochs, or QSBR). Rebalancing with strict time bounds is… a beast.

Practical guidance & pitfalls

Decide: plain BST or balanced? Balancing (AVL/Red-Black) adds rotations that involve a small, fixed set of nodes. This is still fine with lock coupling or optimistic + validation, but significantly harder for lock-free.

Lock ordering is law. Always acquire locks top-down and in a consistent left-to-right order when touching siblings. Release as soon as it’s safe. This prevents deadlocks.

Bound the locked region. For insert/delete, you usually only need to lock the parent and the node you’re changing (plus a sibling/child for rotations). Don’t hold a long path if you can avoid it.

Range queries need stability. If you’ll do in-order traversals or range scans, consider versioned nodes and validation at visit time, or snapshotting (RCU) so readers don’t see “teleporting” keys.

Memory reclamation is not optional. In non-GC languages, deleted nodes can’t be freed immediately if readers may still see them. Use epochs or hazard pointers to retire safely. This is mandatory for optimistic or lock-free designs.

Measure contention patterns. If your hot spots are near the root (common with skewed keys), even fine-grained locking bottlenecks there. Randomized balancing, key hashing, or a higher-fanout structure (B-tree variants) reduces root pressure.

Sometimes the answer is: not a tree. Concurrent skip lists, B-trees/B+-trees, or shard-by-range with per-shard trees can be simpler and scale better, plus they naturally support range scans.

A sane upgrade path

Start with per-node mutexes and lock coupling (top-down, parent then child, release parent). Handle rotations by locking the minimal local neighborhood with a fixed order.

If reads dominate, switch node locks to RW-locks.

If you need more read speed and can tolerate retry logic, add optimistic traversal with version validation.

For ultra-fast reads or lots of range queries, consider RCU-style path copying.

Only if you truly need it, explore lock-free BSTs with proper reclamation.

Tidy mental model: keep independent subtrees independent. Concurrency is just the art of not grabbing more than you need, for longer than you need it. If you want, I can sketch the exact lock/validate sequences for insert, delete, and a single/double rotation in a balanced tree—no code, just the choreography.
*/

// BTNode_ts *bt_find_parent_rec_ts(BTNode_ts *node, pthread_mutex_t *mtx, char *child_key)
// {
//     int s;
//
//     LOCK(node->mtx, s, NULL);
//     UNLOCK(mtx, s, NULL);
//
//     if (node->left != NULL && strcmp(node->left->key, child_key) == 0 ||
//             node->right != NULL && strcmp(node->right->key, child_key) == 0) {
//         UNLOCK(node->mtx, s, NULL);
//         return node;
//     }
//     else {
//         BTNode_ts *nextDirection;
//
//         int strcmp_rv = strcmp(child_key, node->key);
//         if (strcmp_rv < 0)
//             nextDirection = node->left;
//         else if (strcmp_rv > 0)
//             nextDirection = node->right;
//         else {
//             UNLOCK(mtx, s, NULL);
//             return NULL;
//         }
//
//         if (nextDirection == NULL) {
//             UNLOCK(mtx, s, NULL);
//             return NULL;
//         } else
//             return bt_find_parent_rec_ts(nextDirection, node->mtx, child_key);
//     }
// }
//
// int bt_find_parent_ts(BTree_ts *tree, char *child_key)
// {
//     int rv;
//     int s;
//
//     LOCK(tree->mtx, s, -1);
//
//     if (tree->root == NULL) {
//         UNLOCK(tree->mtx, s, -1);
//         return -1;
//     }
//     else
//         rv = bt_add_rec_ts(tree->root, tree->mtx, key, value);
//
//     return rv;
// }
