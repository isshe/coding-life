#ifndef _SINGLE_LINKLIST_H_
#define _SINGLE_LINKLIST_H_

struct slist_node
{
    void *data;
    struct slist_node *next;
};

struct slist_head
{
    struct slist_node *head;
    struct slist_node *tail;
};

typedef int (*compare_func_t)(const void*, const void*);
typedef int (*process_func_t)(const void*);

void slist_init(struct slist_head *head);
void slist_append(struct slist_head *head, struct slist_node *node);
void slist_prepend(struct slist_head *head, struct slist_node *node);
struct slist_node * slist_delete(struct slist_head *head, compare_func_t func, const void *key);
struct slist_node *slist_find(struct slist_head *head, compare_func_t func, const void *key);
int slist_traversal(struct slist_head *head, process_func_t func);

#endif