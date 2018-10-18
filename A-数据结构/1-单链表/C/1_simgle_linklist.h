#ifndef _SIMGLE_LINKLIST_H_
#define _SIMGLE_LINKLIST_H_
// 此写法有头结点（不存数据）

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR,TYPE,MEMBER)    ({  \
    const typeof(((TYPE *)0)->MEMBER) *__mptr=(PTR);  \
    (TYPE *) ((char *)__mptr - offsetof(TYPE,MEMBER)); })

struct list_head 
{
    struct list_head *next;
};

void list_add(struct list_head *pre_node, struct list_head *new_node);
void list_del(struct list_head *head_node, struct list_head *del_node);

struct something 
{
    int val;
    struct list_head list;
};

void print_something(struct list_head *head);



#endif