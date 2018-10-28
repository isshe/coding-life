

#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"
typedef struct rbtree_test_s
{
    rbtree_node_t node;
    int val;
}rbtree_test_t;

int compare(rbtree_node_t *node1, rbtree_node_t *node2)
{
    rbtree_test_t *test1 = container_of(node1, rbtree_test_t, node);
    rbtree_test_t *test2 = container_of(node2, rbtree_test_t, node);

    //printf("test1->val = %d, test2->val = %d\n", test1->val, test2->val);
    return test2->val - test1->val;
}

int traversal(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    if (node == sentinel) {
        printf(" ,");
        return RBTREE_OK;
    }

    rbtree_test_t *test = container_of(node, rbtree_test_t, node);

    printf("%d:%d,", test->val, node->color);
    return RBTREE_OK;
}

rbtree_test_t *new_test_node(int val)
{
    rbtree_test_t *node = NULL;

    node = (rbtree_test_t *)malloc(sizeof(rbtree_test_t));
    if (!node) {
        return NULL;
    }

    node->val = val;
    return node;
}

int main()
{
    rbtree_t *tree = rbtree_create(NULL, compare);
    tree->same_flag = 0;

    rbtree_test_t *test1 = new_test_node(5);
    rbtree_test_t *test2 = new_test_node(2);
    rbtree_test_t *test3 = new_test_node(10);
    rbtree_test_t *test4 = new_test_node(7);
    rbtree_test_t *test5 = new_test_node(9);
    rbtree_test_t *test6 = new_test_node(6);
    rbtree_test_t *test7 = new_test_node(8);

    rbtree_insert(tree, &test1->node);
    rbtree_insert(tree, &test2->node);
    rbtree_insert(tree, &test3->node);
    rbtree_insert(tree, &test4->node);
    rbtree_insert(tree, &test5->node);
    rbtree_insert(tree, &test6->node);
    rbtree_insert(tree, &test7->node);

    rbtree_layer_traversal(tree, traversal);

    //rbtree_inorder_traversal(tree->root, traversal);
    printf("\n");

    rbtree_delete2(tree, &test4->node);

    //rbtree_inorder_traversal(tree->root, traversal);
    rbtree_layer_traversal(tree, traversal);

    printf("\n");

    return 0;
}