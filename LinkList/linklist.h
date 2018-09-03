#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

struct list_head
{
    //struct list_head *prev; //前级指针
    struct list_head *next;//后级指针
};

struct element
{
    int val;
    struct list_head list;
};

#endif