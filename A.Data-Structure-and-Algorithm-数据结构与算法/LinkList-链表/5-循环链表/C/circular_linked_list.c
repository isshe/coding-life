#include "circular_linked_list.h"

#include <malloc.h>
#include <stdlib.h>

circular_linked_list_t *circular_linked_list_create()
{
    circular_linked_list_t *list = (circular_linked_list_t *)malloc(sizeof(circular_linked_list_t));
    if(!list) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void circular_linked_list_destroy(circular_linked_list_t *list)
{
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        free(list);
        list = NULL;
    }
}

void circular_linked_list_add(circular_linked_list_t *list, circular_linked_node_t *node)
{
    if (list->size == 0) {
        node->next = node;
        list->head = node;
        list->tail = list->head;
    } else {
        node->next = list->head;
        list->head = node;
        list->tail->next = list->head;
    }

    list->size++;
}

void circular_linked_list_del(circular_linked_list_t *list, circular_linked_node_t *node)
{
    if (list->size == 0) {
        return;
    }

    circular_linked_node_t *cur_node = list->head;
    circular_linked_node_t *pre_node = list->tail;
    while(cur_node != NULL) {
        if (cur_node == node) {
            pre_node->next = cur_node->next;
            if (cur_node == list->head) {
                list->head = pre_node->next;
            }

            if (cur_node == list->tail) {
                list->tail = pre_node;
            }

            list->size--;
            break;
        }

        pre_node = cur_node;
        cur_node = cur_node->next;
        if (cur_node == list->head) {
            break;
        }
    }
}

unsigned int circular_linked_list_size(circular_linked_list_t *list)
{
    return list->size;
}

// print、find、之类的都可以通过这个函数来实现
void circular_listed_list_traverse(circular_linked_list_t *list, traverse_func func, void *func_args)
{
    if (list->size == 0) {
        return;
    }

    circular_linked_node_t *cur_node = list->head;
    while(cur_node != NULL) {
        func(cur_node, func_args);

        cur_node = cur_node->next;
        if (cur_node == list->head) {
            break;
        }
    }
}

circular_linked_node_t *circular_listed_list_find(circular_linked_list_t *list, circular_linked_node_t *node)
{
    if (list->size == 0) {
        return NULL;
    }

    circular_linked_node_t *res_node = NULL;
    circular_linked_node_t *cur_node = list->head;
    while(cur_node != NULL) {
        if (cur_node == node) {
            res_node = cur_node;
            break;
        }

        cur_node = cur_node->next;
        if (cur_node == list->head) {
            break;
        }
    }
    return res_node;
}
 