
#include "rbtree.h"

int is_red(rbtree_node_t *node)
{
    if (!node)
    {
        return RBTREE_BLACK;
    }

    return node->color;
}