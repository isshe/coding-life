
#ifndef DOUBLY_LINKLIST_H_
#define DOUBLY_LINKLIST_H_

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)


struct list_head 
{
    struct list_head *prev;
    struct list_head *next;
}

void init_list_head(struct list_head *list);

void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next);

void list_add(struct list_head *new, struct list_head *head);

void list_add_tail(struct list_head *new, struct list_head *head);

void __list_del(struct list_head *prev, struct list_head *next);

void list_del(struct list_head *entry);

void list_replace(struct list_head *old, struct list_head *new);

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
void list_is_last(const struct list_head *list, const struct list_head *head);

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
void list_is_empty(const struct list_head *head);
                
                
#endif