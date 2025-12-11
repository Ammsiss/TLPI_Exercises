#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#define BT_KEY_MAX 255

#define ROOT 0
#define LEFT_TRAVERSAL 1
#define RIGHT_TRAVERSAL 2

typedef struct BTNode BTNode;

struct BTNode {
    BTNode *left;
    BTNode *right;
    char *key;
    void *value;
};

typedef struct {
    BTNode *root;
} BTree;

// Set root pointer to null
void bt_initialize(BTree *tree);
// Allocate a new node with 'key' and 'value' and store its
// address in 'node'
int bt_add(BTree *tree, char *key, void *value);
// Return the address of the node who holds a pointer against
// 'child_key' starting traversal from 'node'
BTNode *bt_find_parent(BTNode *node, char *child_key);
// Store the pointer that corresponds to the value of 'key' in
// 'value', starting traversal from 'node'
int bt_lookup(BTree *tree, char *key, void **value);
// Delete node assocaited with 'target_key' in 'tree'
int bt_delete(BTree *tree, char *target_key);
// Deallocate the entire tree passed in as 'tree'
void bt_free(BTree *tree);
// Print an ascii art representation of 'tree'
void bt_print(BTree *tree);

#endif
