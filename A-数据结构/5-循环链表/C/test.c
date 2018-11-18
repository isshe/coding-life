#include "circular_linked_list.h"

#include <stdio.h>
#include <malloc.h>

typedef struct test {
    int val;
    circular_linked_node_t node;
}test_t;

test_t *new_test_node(int val)
{
    test_t *new_node = (test_t *)malloc(sizeof(test_t));
    if (!new_node) {
        return NULL;
    }
    new_node->val = val;
    return new_node;
}

void test_node_print(circular_linked_node_t *cur_node, void *args)
{
    test_t *tnode = container_of(cur_node, test_t, node);
    printf("%d, ", tnode->val);
}

void print_find_res(circular_linked_list_t *list, circular_linked_node_t *node)
{
    if (circular_listed_list_find(list, node)) {
        printf("found!!!\n");
    } else {
        printf("not found!!!\n");
    }
}

int main(void)
{
    circular_linked_list_t *list = circular_linked_list_create();
    test_t *tnode_1 = new_test_node(1);
    test_t *tnode_2 = new_test_node(2);
    circular_linked_list_add(list, &tnode_1->node);
    circular_linked_list_add(list, &tnode_2->node);
    circular_listed_list_traverse(list, test_node_print, NULL);
    printf("\n");

    test_t *tnode_3 = new_test_node(3);
    circular_linked_list_add(list, &tnode_3->node);
    test_t *tnode_4 = new_test_node(4);
    circular_linked_list_add(list, &tnode_4->node);
    circular_listed_list_traverse(list, test_node_print, NULL);
    printf("\n");

    circular_linked_list_del(list, &tnode_2->node);
    free(tnode_2);
    circular_listed_list_traverse(list, test_node_print, NULL);
    printf("\n");

    circular_linked_list_del(list, &tnode_3->node);
    free(tnode_3);
    circular_listed_list_traverse(list, test_node_print, NULL);
    printf("\n");

    circular_linked_list_del(list, &tnode_1->node);
    circular_listed_list_traverse(list, test_node_print, NULL);
    printf("\n");

    print_find_res(list, &tnode_1->node);
    print_find_res(list, &tnode_4->node);
    free(tnode_1);

    printf("list size = %d\n", circular_linked_list_size(list));
    circular_linked_list_del(list, &tnode_4->node);
    free(tnode_4);
    printf("list size = %d\n", circular_linked_list_size(list));

    circular_linked_list_destroy(list);
    return 0;
}