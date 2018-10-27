

#include <stdio.h>

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

    printf("test1->val = %d, test2->val = %d\n", test1->val, test2->val);
    return test2->val - test1->val;
}

int traversal(rbtree_node_t *node)
{
    rbtree_test_t *test = container_of(node, rbtree_test_t, node);

    printf("%d,", test->val);
}

int main()
{
    rbtree_t *tree = rbtree_create(NULL, compare);
    tree->same_flag = 0;

    rbtree_test_t test1;
    rbtree_test_t test2;
    rbtree_test_t test3;

    test1.val = 1;
    test2.val = 2;
    test3.val = 3;

    rbtree_insert(tree, &test2.node);
    printf("tree->size = %d\n", (int)tree->size);
    rbtree_insert(tree, &test3.node);
    printf("tree->size = %d\n", (int)tree->size);
    rbtree_insert(tree, &test1.node);
    printf("tree->size = %d\n", (int)tree->size);

    rbtree_inorder_traversal(tree->root, traversal);
    printf("\n");

    rbtree_delete(tree, &test3.node);

    rbtree_inorder_traversal(tree->root, traversal);
    printf("\n");

    return 0;
}