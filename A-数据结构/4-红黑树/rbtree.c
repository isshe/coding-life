
#include <assert.h>

#include "rbtree.h"

#define RBTREE_NIL &_sentinel /* all leafs are sentinels */
static rbtree_node_t _sentinel = {RBTREE_NIL, RBTREE_NIL, NULL, RBTREE_BLACK};

int is_red(rbtree_node_t *node)
{
    if (!node) {
        return RBTREE_BLACK;
    }

    return node->color;
}

rbtree_t *rbtree_create(rbtree_compare_func compare_func)
{
    rbtree_t *tree = NULL;

    tree = (rbtree_t *)malloc(sizeof(rbtree_t));
    if (!tree) {
        return NULL;
    }

    tree->size = 0;
    tree->root = RBTREE_NIL;
    tree->compare_func = compare_func;

    return tree;
}


/**
 *     [P]                          [P]
 *      |                            |
 *     [N]                          [T]
 *     / \         ->               / \
 *    [1][T]                      [N] [3]
 *       / \                      / \
 *      [2][3]                   [1][2]
 */
void rbtree_left_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->right;

    // 更新x的right，和y->left的parent
    node->right = temp->left;
    if (temp->left != RBTREE_NIL) {
        temp->left->parent = node;
    }

    // 重新建立y的parent链接
    // 1.更新temp的父节点
    temp->parent = node->parent;

    // 2.更新原来x的父节点的子节点，或者更新根节点
    if (node->parent) { // 根节点的parent = NULL
        if (node == node->parent->left) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    } else {
        tree->root = temp;
    }

    //更新y的left和x的parent
    temp->left = node;
    node->parent = temp;
}


/**
 *     [P]             [P]            [P]               [P]
 *      |               |              |                 |
 *     [N]   ->    [T] [N]     ->     [T] [N]    ->     [T]
 *     / \         / \ / \            / \ / \           / \
 *    [T][1]      [2][3][1]          [2][3] [1]       [2] [N]
 *    / \                                                 / \
 *   [2][3]                                              [3][1]
 */
void rbtree_right_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->left;

    // 更新node->left
    node->left = temp->right;
    if (temp->right != RBTREE_NIL) {
        temp->right->parent = node;
    }

    // 更新temp->parent
    temp->parent = node->parent;

    if(node->parent) {
        if (node == node->parent->left) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    } else {
        tree->root = temp;
    }

    // 更新temp->right
    temp->right = node;
    node->parent = temp;
}
