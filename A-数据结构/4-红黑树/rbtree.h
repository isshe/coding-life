
#ifndef RBTREE_H_
#define RBTREE_H_

#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR,TYPE,MEMBER)    ({  \
    const typeof(((TYPE *)0)->MEMBER) *__mptr=(PTR);  \
    (TYPE *) ((char *)__mptr - offsetof(TYPE,MEMBER)); })


enum rbstatus_e
{
    RBTREE_OK = 0,
    RBTREE_ERROR
};
typedef enum rbstatus_e rbstatus_t;

#define rbtree_increace(tree)       ((tree)->size++)
#define rbtree_decreace(tree)       ((tree)->size--)

enum rbcolor_e 
{ 
    RBTREE_BLACK = 0, 
    RBTREE_RED 
};
typedef enum rbcolor_e rbcolor_t;

#define rbnode_red(node)               ((node)->color = RBTREE_RED)
#define rbnode_black(node)             ((node)->color = RBTREE_BLACK)
#define rbnode_is_red(node)            ((node)->color)
#define rbnode_is_black(node)          (!rbnode_is_red(node))
#define rbnode_copy_color(n1, n2)      (n1->color = n2->color)

typedef struct rbtree_node_s  rbtree_node_t;
struct rbtree_node_s {
    rbtree_node_t       *left;
    rbtree_node_t       *right;
    rbtree_node_t       *parent;
    rbcolor_t           color;
};

enum rbsame_e
{
    RBSAME_RETURN = 0,
    RBSAME_LEFT ,
    RBSAME_RIGHT
};
typedef enum rbsame_e rbsame_t;

typedef struct rbtree_s rbtree_t;

typedef int (*rbtree_compare_func) (rbtree_node_t *node1, rbtree_node_t *node2);
typedef int (*rbtree_insert_func) (rbtree_t *tree, rbtree_node_t *node, rbtree_node_t *sentinel);
typedef int (*traversal_func)(rbtree_node_t *node);

struct rbtree_s {
    size_t size;                        // 记录树的大小
    rbtree_node_t     *root;            // 树的根
    rbtree_compare_func compare_func;   // 比较函数
    rbtree_insert_func insert_func;     // 插入函数
    rbsame_t same_flag;
};

int is_red(rbtree_node_t *node);

rbtree_node_t *rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel);
rbtree_node_t *rbtree_max(rbtree_node_t *node, rbtree_node_t *sentinel);
rbtree_t *rbtree_create(rbtree_insert_func insert_func, rbtree_compare_func compare_func);
int rbtree_insert(rbtree_t *tree, rbtree_node_t *node);
void rbtree_delete(rbtree_t *tree, rbtree_node_t *node);

void rbtree_inorder_traversal(rbtree_node_t *root, traversal_func func);


#endif