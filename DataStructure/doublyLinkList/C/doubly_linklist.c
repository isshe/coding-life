
#include "doubly_linklist.h"

void init_list_head(struct list_head *list)
{
    list->prev = list;
    list->next = list;
}

void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

// 加载head后面
void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

// 加载head前面
void list_add_tail(struct list_head *new, struct list_head *head)
{
     __list_add(new, head->prev, head);
}

void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->prev = NULL;
    entry->next = NULL;
}

void list_replace(struct list_head *old, struct list_head *new)
{
    new->prev = old->prev;
    new->next = old->next;
    new->prev->next = new;
    new->next->prev = new;
}

void list_is_last(const struct list_head *list, const struct list_head *head)
{
    return list->next == head;
}

void list_is_empty(const struct list_head *head)
{
    return head->next == head;
}

