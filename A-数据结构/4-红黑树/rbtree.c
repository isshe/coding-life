
#include <assert.h>

#include "rbtree.h"

#define RBTREE_NIL &_sentinel /* all leafs are sentinels */
static rbtree_node_t _sentinel = {RBTREE_NIL, RBTREE_NIL, NULL, RBTREE_BLACK};

int is_red(rbtree_node_t *node)
{
    if (!node) {
        return RBTREE_BLACK;
    }

    return node->color;
}

rbtree_t *rbtree_create(rbtree_insert_func insert_func, rbtree_compare_func compare_func)
{
    rbtree_t *tree = NULL;

    tree = (rbtree_t *)malloc(sizeof(rbtree_t));
    if (!tree) {
        return NULL;
    }

    tree->size = 0;
    tree->root = RBTREE_NIL;
    tree->compare_func = compare_func;
    tree->insert_func = insert_func;
    tree->same_flag = RBSAME_RETURN;

    return tree;
}

/**
 *     [P]                          [P]
 *      |                            |
 *     [N]                          [T]
 *     / \         ->               / \
 *    [1][T]                      [N] [3]
 *       / \                      / \
 *      [2][3]                   [1][2]
 */
void rbtree_left_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->right;

    // 更新x的right，和y->left的parent
    node->right = temp->left;
    if (temp->left != RBTREE_NIL) {
        temp->left->parent = node;
    }

    // 重新建立y的parent链接
    // 1.更新temp的父节点
    temp->parent = node->parent;

    // 2.更新原来x的父节点的子节点，或者更新根节点
    if (node->parent) { // 根节点的parent = NULL
        if (node == node->parent->left) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    } else {
        tree->root = temp;
    }

    //更新y的left和x的parent
    temp->left = node;
    node->parent = temp;
}


/**
 *     [P]             [P]            [P]               [P]
 *      |               |              |                 |
 *     [N]   ->    [T] [N]     ->     [T] [N]    ->     [T]
 *     / \         / \ / \            / \ / \           / \
 *    [T][1]      [2][3][1]          [2][3] [1]       [2] [N]
 *    / \                                                 / \
 *   [2][3]                                              [3][1]
 */
void rbtree_right_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->left;

    // 更新node->left
    node->left = temp->right;
    if (temp->right != RBTREE_NIL) {
        temp->right->parent = node;
    }

    // 更新temp->parent
    temp->parent = node->parent;

    if(node->parent) {
        if (node == node->parent->left) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    } else {
        tree->root = temp;
    }

    // 更新temp->right
    temp->right = node;
    node->parent = temp;
}


int temp_compare(rbtree_node_t *node1, rbtree_node_t *node2)
{

}

int binary_tree_insert(rbtree_t *tree, rbtree_node_t *node, rbtree_node_t *sentinel)
{
    rbtree_node_t *temp, *parent;
    int cmp = 0;

    // 树空，直接插入后设为BLACK，返回
    temp = tree->root;
    parent = NULL;
    while (temp != sentinel) {
        cmp = tree->compare_func(temp, node);
        parent = temp;
        if (cmp == 0) {                         // 这里需要再考虑一下怎么处理，相同的值
            //return RBTREE_ERROR;                // same value
            switch(tree->same_flag)
            {
                case RBSAME_LEFT:
                    temp = temp->left;
                    break;
                case RBSAME_RIGHT:
                    temp = temp->right;
                    break;
                case RBSAME_RETURN:
                default:
                    return RBTREE_ERROR;
            }
        } else if (cmp < 0) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }

    node->left = sentinel;
    node->right = sentinel;
    node->parent = parent;

    if (cmp < 0) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    return RBTREE_OK;
}

int rbtree_insert(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp, *sentinel;
    int cmp = 0;
    int rc = 0;

    /* a binary tree insert */
    sentinel = RBTREE_NIL;
    if (tree->root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        rbnode_black(node);
        tree->root = node;

        return RBTREE_OK;
    }

    // 这里是自定义的插入，node 为 RED
    rbnode_red(node);
    if (tree->insert_func) {
        rc = tree->insert_func(tree, node, sentinel);
    } else {
        rc = binary_tree_insert(tree, node, sentinel);
    }
    if (rc != RBTREE_OK)
    {
        return rc;
    }

    /* re-balance tree */
    // 当前节点node为RED，如果父节点也是RED，证明父节点不是根节点，那就至少还有3层
    while (node != tree->root && rbnode_is_red(node->parent)) {

        // 父节点是父父节点的左节点
        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            // 父节点及其兄弟节点都为红
            if (rbnode_is_red(temp)) {                                  /*     [pp:b]                [pp:r](new n）*/
                rbnode_black(node->parent);                             /*      /  \                  /  \         */ 
                rbnode_black(temp);                                     /*   [p:r] [t:r]   (换色)   [p:b] [t:b]     */
                rbnode_red(node->parent->parent);                       /*     |                     |             */
                node = node->parent->parent;                            /*   [n:r]                 [n:r]           */

            } else {                                                    /*     [pp:b]                [pp:b]        */
                if (node == node->parent->right) {                      /*      /  \                  /  \         */
                    node = node->parent;                                /*   [p:r] [t:b]   (左旋)   [p:r] [t:b]     */
                    rbtree_left_rotate(tree, node);                     /*      \                   /              */
                }                                                       /*     [n:r]              [n:r]            */

                rbnode_black(node->parent);                             /*     [pp:b]                [p:b]         */
                rbnode_red(node->parent->parent);                       /*      /  \                  /  \         */
                rbtree_right_rotate(tree, node->parent->parent);        /*   [p:r] [t:b]   (右旋)   [n:r] [pp:r]    */
            }                                                           /*    / \                           \      */
                                                                        /*  [n:r]                          [t:b]   */
        } else {
            temp = node->parent->parent->left;

            if (rbnode_is_red(temp)) {                                  /*     [pp:b]                [pp:r](new n）*/
                rbnode_black(node->parent);                             /*      /  \                  /  \         */
                rbnode_black(temp);                                     /*   [t:r] [p:r]   (换色)   [t:b] [p:b]     */
                rbnode_red(node->parent->parent);                       /*           |                     |       */
                node = node->parent->parent;                            /*         [n:r]                 [n:r]     */

            } else {                                                    /*     [pp:b]                [pp:b]        */
                if (node == node->parent->left) {                       /*      /  \                  /  \         */
                    node = node->parent;                                /*   [t:b] [p:r]   (右旋)   [t:b] [p:r]     */
                    rbtree_right_rotate(tree, node);                    /*          /                       \      */
                }                                                       /*       [n:r]                     [n:r]   */

                rbnode_black(node->parent);                             /*     [pp:b]                [p:b]         */
                rbnode_red(node->parent->parent);                       /*      /  \                  /  \         */
                rbtree_left_rotate(tree, node->parent->parent);         /*   [t:b] [p:r]   (左旋)   [pp:r] [n:r]    */
            }                                                           /*           \              /              */
        }                                                               /*          [n:r]        [t:b]             */
    }

    rbnode_black(tree->root);
    rbtree_increace(tree);
    return RBTREE_OK;
}

rbtree_node_t *rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

rbtree_node_t *rbtree_max(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while (node->right != sentinel) {
        node = node->right;
    }

    return node;
}

void rbtree_delete(rbtree_t *tree, rbtree_node_t *node)
{
    rbcolor_t red;
    rbtree_node_t   **root;
    rbtree_node_t   *sentinel;
    rbtree_node_t   *subst;             // 将来要替换被删除节点的节点
    rbtree_node_t   *temp;
    rbtree_node_t   *w;

    /* a binary tree delete */
    sentinel = RBTREE_NIL;

    //root = (ngx_rbtree_node_t **) &tree->root;

    // 要删除的节点只有一个右子节点或者没有子节点
    if (node->left == sentinel) {
        temp = node->right;
        subst = node;
    // 要删除的节点只有一个左子节点
    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;
    // 要删除的节点有两个子节点
    } else {
        // 找到右子树中最小的一个节点，用来代替将被删除的节点
        // 也可以找左子树中最大的节点
        subst = rbtree_min(node->right, sentinel);

        // 这个不可能成立了吧？多余？
        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    // 条件成立的话 subst == node
    // 被删除的节点是根节点且只有至多一个儿子，把下一个节点替换上来即可
    if (subst == tree->root) {
        tree->root = temp;
        if (temp != sentinel)
        {
            rbnode_black(temp);
        }

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;

        return;
    }

    // 下面是 subst 和 node 都不是 根节点的情况
    // 代替节点是否是红节点
    red = rbnode_is_red(subst);

    // temp有可能是被删除节点的子节点，
    // 也可能是左子树的最右(大)节点的左节点、或者右子树最左(小)节点的右节点
    // 将temp替换到将被删除的节点的位置
    if (subst == subst->parent->left) {
        subst->parent->left = temp;
    } else {
        subst->parent->right = temp;
    }

    // 被删的节点至多有一个儿子
    if (subst == node) {
        temp->parent = subst->parent;       // 这里temp有可能是sentinel?! node没有儿子节点时

    // 被删的节点有两个儿子
    } else {

        // node是根节点
        if (subst->parent == node) {
            temp->parent = subst;           // 这里temp有可能是sentinel?! subst没有节点时

        } else {
            temp->parent = subst->parent;   // 这里temp有可能是sentinel?! subst没有节点时
        }

        // 用subst代替node
        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        rbnode_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *root && ngx_rbt_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } else {
                if (ngx_rbt_is_black(w->right)) {
                    ngx_rbt_black(w->left);
                    ngx_rbt_red(w);
                    ngx_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->right);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            if (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } else {
                if (ngx_rbt_is_black(w->left)) {
                    ngx_rbt_black(w->right);
                    ngx_rbt_red(w);
                    ngx_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->left);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    ngx_rbt_black(temp);
}
