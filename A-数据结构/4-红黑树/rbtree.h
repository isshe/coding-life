
#ifndef RBTREE_H_
#define RBTREE_H_

#define RBTREE_RED    1
#define RBTREE_BLACK  0

typedef struct rbtree_node_s  rbtree_node_t;
struct rbtree_node_s {
    rbtree_node_t       *left;
    rbtree_node_t       *right;
    rbtree_node_t       *parent;
    unsigned char       color;
};

typedef int (*rbtree_compare_func) (rbtree_node_t *node1, rbtree_node_t *node2);

typedef struct rbtree_s  rbtree_t;
struct rbtree_s {
    rbtree_node_t     *root;
    rbtree_compare_func compare;
};


int is_red(rbtree_node_t *node);

#endif