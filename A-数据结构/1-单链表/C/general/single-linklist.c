
#include "single-linklist.h"

#include <stdio.h>

void slist_init(struct slist_head *head)
{
    head->head = NULL;
    head->tail = NULL;
}

void slist_append(struct slist_head *head, struct slist_node *node)
{
    node->next = NULL;
    if (head->tail) {
        head->tail->next = node;
    }

    head->tail = node;

    if (!head->head) {
        head->head = node;
    }
}

void slist_prepend(struct slist_head *head, struct slist_node *node)
{
    node->next = head->head;
    head->head = node;
    if (!head->tail) {
        head->tail = node;
    }
}

struct slist_node * slist_delete(struct slist_head *head, compare_func_t func, const void *key)
{
    struct slist_node *pre_node;
    struct slist_node *node;

    for (pre_node = NULL, node = head->head; node ; node = node->next) {
        if (func(node->data, key) == 0) {
            if (pre_node) {
                pre_node->next = node->next;
            }

            if (head->tail == node) {
                head->tail = pre_node;
            }

            if (head->head == node) {
                head->head = node->next;
            }

            return node;
        }

        pre_node = node;
    }

    return NULL;
}

struct slist_node *slist_find(struct slist_head *head, compare_func_t func, const void *key)
{
    struct slist_node *node;

    for (node = head->head; node ; node = node->next) {
        if (func(node->data, key) == 0) {
            return node;
        }
    }

    return NULL;
}

int slist_traversal(struct slist_head *head, process_func_t func)
{
    struct slist_node *node;
    int rc = 0;

    for (node = head->head; node ; node = node->next) {
        if ((rc = func(node->data)) != 0) {
            return rc;
        }
    }

    return rc;
}