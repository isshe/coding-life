
#ifndef RBTREE_H_
#define RBTREE_H_

#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR,TYPE,MEMBER)    ({  \
    const typeof(((TYPE *)0)->MEMBER) *__mptr=(PTR);  \
    (TYPE *) ((char *)__mptr - offsetof(TYPE,MEMBER)); })

enum rbcolor_e 
{ 
    RBTREE_BLACK = 0, 
    RBTREE_RED 
}; 
typedef enum rbcolor_e rbcolor_t;

typedef struct rbtree_node_s  rbtree_node_t;
struct rbtree_node_s {
    rbtree_node_t       *left;
    rbtree_node_t       *right;
    rbtree_node_t       *parent;
    rbcolor_t           color;
};

typedef int (*rbtree_compare_func) (rbtree_node_t *node1, rbtree_node_t *node2);

typedef struct rbtree_s  rbtree_t;
struct rbtree_s {
    size_t size;                        // 记录树的大小
    rbtree_node_t     *root;            // 树的根
    rbtree_compare_func compare_func;   // 比较函数
};


int is_red(rbtree_node_t *node);

rbtree_t *rbtree_create(rbtree_compare_func compare_func);


#endif