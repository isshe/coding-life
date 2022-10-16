#ifndef CIRCULAR_LINKED_LIST_H_
#define CIRCULAR_LINKED_LIST_H_

#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR,TYPE,MEMBER)    ({  \
    const typeof(((TYPE *)0)->MEMBER) *__mptr=(PTR);  \
    (TYPE *) ((char *)__mptr - offsetof(TYPE,MEMBER)); })


struct circular_linked_node_s {
    struct circular_linked_node_s *next;
};

typedef struct circular_linked_node_s circular_linked_node_t;

struct circular_linked_list_s {
    circular_linked_node_t *head;
    circular_linked_node_t *tail;
    unsigned int size;
};

typedef struct circular_linked_list_s circular_linked_list_t;

typedef void (*traverse_func)(circular_linked_node_t *, void *func_args);

// 创建&初始化
circular_linked_list_t *circular_linked_list_create();

// 头部添加
void circular_linked_list_add(circular_linked_list_t *list, circular_linked_node_t *node);

// 删除
void circular_linked_list_del(circular_linked_list_t *list, circular_linked_node_t *node);

// 获取大小
unsigned int circular_linked_list_size(circular_linked_list_t *list);

// 遍历
void circular_listed_list_traverse(circular_linked_list_t *list, traverse_func func, void *func_args);

// 销毁，注意节点需要使用者来销毁，这里的销毁只是销毁头结点
void circular_linked_list_destroy(circular_linked_list_t *list);

circular_linked_node_t *circular_listed_list_find(circular_linked_list_t *list, circular_linked_node_t *node);

#endif