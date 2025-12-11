#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

static int bt_create_node(BTNode **node, char *key, void *value)
{
    *node = malloc(sizeof(BTNode));

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->value = value;

    (*node)->key = malloc(BT_KEY_MAX + 1);
    if ((*node)->key == NULL)
        return -1;
    strcpy((*node)->key, key);

    return 0;
}

static int bt_add_rec(BTNode *node, char *key, void *value)
{
    int rv = strcmp(key, node->key);
    if (rv == 0) {
        errno = EINVAL;
        return -1;
    }

    BTNode **nextDirection;
    if (rv < 0)
        nextDirection = &node->left;
    else
        nextDirection = &node->right;

    if (*nextDirection == NULL) {
        return bt_create_node(nextDirection, key, value);
    } else {
        return bt_add_rec(*nextDirection, key, value);
    }
}

static BTNode **bt_find_node(BTNode **node, char *key)
{
    BTNode **nextDirection;

    int rv = strcmp(key, (*node)->key);

    if (rv < 0)
        nextDirection = &(*node)->left;
    else if (rv > 0)
        nextDirection = &(*node)->right;
    else {
        return node;
    }

    if (*nextDirection == NULL)
        return NULL;

    return bt_find_node(nextDirection, key);
}

static void bt_free_node(BTNode **node)
{
    free((*node)->key);
    free(*node);
    *node = NULL;
}

static void bt_free_rec(BTNode **node)
{
    if ((*node)->left != NULL)
        bt_free_rec(&(*node)->left);

    if ((*node)->right != NULL)
        bt_free_rec(&(*node)->right);

    bt_free_node(node);
}

static void bt_print_rec(BTNode *node, int traversal)
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

        bt_print_rec(node->right, RIGHT_TRAVERSAL);

        space_count -= strlen(buf);
    }

    if (node->left != NULL)
        bt_print_rec(node->left, LEFT_TRAVERSAL);
}

void bt_initialize(BTree *tree)
{
    tree->root = NULL;
}

int bt_add(BTree *tree, char *key, void *value)
{
    if (tree->root == NULL)
        return bt_create_node(&tree->root, key, value);

    return bt_add_rec(tree->root, key, value);
}

BTNode *bt_find_parent(BTNode *node, char *child_key)
{
    if (node->left != NULL && strcmp(node->left->key, child_key) == 0)
        return node;
    else if (node->right != NULL && strcmp(node->right->key, child_key) == 0)
        return node;

    BTNode *nextDirection;

    int rv = strcmp(child_key, node->key);
    if (rv < 0)
        nextDirection = node->left;
    else if (rv > 0)
        nextDirection = node->right;
    else
        return NULL;

    if (nextDirection == NULL)
        return NULL;

    return bt_find_parent(nextDirection, child_key);
}


int bt_lookup(BTree *tree, char *key, void **value)
{
    if (tree->root == NULL)
        return -1;

    BTNode *target = *bt_find_node(&tree->root, key);
    if (target == NULL)
        return -1;


    *value = target->value;

    return 0;
}

int bt_delete(BTree *tree, char *target_key)
{
    if (tree->root == NULL)
        return -1;

    BTNode **target = bt_find_node(&tree->root, target_key);
    if (target == NULL) {
        errno = EINVAL;
        return -1;
    }

    BTNode *target_left = (*target)->left;
    BTNode *target_right = (*target)->right;

    int num_children = 0;
    if (target_left != NULL)
        ++num_children;
    if (target_right != NULL)
        ++num_children;

    switch (num_children) {
    case 0:
        bt_free_node(target);
        break;
    case 1:
        bt_free_node(target);

        if (target_left)
            *target = target_left;
        else
            *target = target_right;
        break;
    case 2:
    {
        BTNode **delete_node = &(*target)->right;
        while (1) {
            if ((*delete_node)->left == NULL)
                break;
            delete_node = &(*delete_node)->left;
        }

        (*target)->value = (*delete_node)->value;
        strcpy((*target)->key, (*delete_node)->key);

        BTNode *delete_node_child = (*delete_node)->right;

        bt_free_node(delete_node);

        *delete_node = delete_node_child;

        break;
    }
    }

    return 0;
}

void bt_free(BTree *tree)
{
    if (tree->root == NULL)
        return;

    bt_free_rec(&tree->root);
}

void bt_print(BTree *tree)
{
    if (tree->root == NULL) {
        printf("Tree is empty.\n");
        return;
    }

    bt_print_rec(tree->root, ROOT);
    printf("\n");
}
