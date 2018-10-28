
#include <assert.h>
#include <stdlib.h>

#include "rbtree.h"

static void
rbtree_init_sentinel(rbtree_node_t *sentinel)
{
    sentinel->parent = NULL;
    sentinel->left = NULL;
    sentinel->right = NULL;
    rbnode_black(sentinel);
}

static rbtree_node_t *
rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

static rbtree_node_t *
rbtree_max(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while (node->right != sentinel) {
        node = node->right;
    }

    return node;
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
static void
rbtree_left_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->right;

    // 更新x的right，和y->left的parent
    node->right = temp->left;
    if (temp->left != tree->sentinel) {
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
static void
rbtree_right_rotate(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;

    assert(tree);
    assert(node);

    temp = node->left;

    // 更新node->left
    node->left = temp->right;
    if (temp->right != tree->sentinel) {
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

rbtree_t *
rbtree_create(rbtree_insert_func insert_func, rbtree_compare_func compare_func)
{
    rbtree_t *tree = NULL;


    tree = (rbtree_t *)malloc(sizeof(rbtree_t));
    if (!tree) {
        return NULL;
    }

    tree->sentinel = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
    if (!tree->sentinel) {
        return NULL;
    }

    rbtree_init_sentinel(tree->sentinel);
    tree->size = 0;
    tree->root = tree->sentinel;
    tree->compare_func = compare_func;
    tree->insert_func = insert_func;
    tree->same_flag = RBSAME_RETURN;

    return tree;
}

static int
binary_tree_insert(rbtree_t *tree, rbtree_node_t *node, rbtree_node_t *sentinel)
{
    rbtree_node_t *temp, *parent;
    int cmp = 0;

    // 树空，直接插入后设为BLACK，返回
    temp = tree->root;
    parent = NULL;
    while (temp != sentinel) {
        cmp = tree->compare_func(temp, node);
        //printf("cmp result = %d\n", cmp);
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

int
rbtree_insert(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *temp, *sentinel;
    int cmp = 0;
    int rc = 0;

    /* a binary tree insert */
    sentinel = tree->sentinel;
    if (tree->root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        rbnode_black(node);
        tree->root = node;

        printf("insert root...\n");
        rbtree_increace(tree);
        return RBTREE_OK;
    }

    // 这里是自定义的插入，node 为 RED
    rbnode_red(node);
    if (tree->insert_func) {
        //printf("using user insert function...\n");
        rc = tree->insert_func(tree, node, sentinel);
    } else {
        //printf("using user default function...\n");
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

void
rbtree_delete(rbtree_t *tree, rbtree_node_t *node)
{
    rbcolor_t red;
    rbtree_node_t   **root;
    rbtree_node_t   *sentinel;
    rbtree_node_t   *subst;             // substitute: 替换
    rbtree_node_t   *temp;
    rbtree_node_t   *w;

    /* a binary tree delete */
    sentinel = tree->sentinel;

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

        rbtree_decreace(tree);
        return;
    }

    red = rbnode_is_red(subst);

    // temp有可能是被删除节点的子节点，
    // 也可能是左子树的最右(大)节点的左节点、或者右子树最左(小)节点的右节点
    // 将temp替换到将被删除的节点的位置
    if (subst == subst->parent->left) {
        subst->parent->left = temp;
    } else {
        subst->parent->right = temp;
    }

    if (temp == sentinel){
        printf("temp == sentinel!!!!!!!!!!!!!!!\n");
    }

    // 被删的节点至多有一个儿子, subst=node被删除（位置被temp替换）
    if (subst == node) {
        if (temp != sentinel) {
            temp->parent = subst->parent;
        }

    // node有两个儿子，用subst代替node
    } else {

        // 此时node的一个儿子已经是temp了
        if (temp != sentinel) {
            if (subst->parent == node) {
                temp->parent = subst;

            } else {
                temp->parent = subst->parent;
            }
        }

        // 用subst代替node
        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        rbnode_copy_color(subst, node);

        // subst已经替换了node，现在让周围的节点指向subst
        // node是根节点
        if (node == tree->root) {
            tree->root = subst;

        // node不是根节点，那它就有父节点
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
    rbtree_decreace(tree);

    if (red) {
        printf("node is red, quick return....\n");
        rbtree_init_sentinel(sentinel);
        return;
    }

    /* a delete fixup */
    while (temp != tree->root && rbnode_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (rbnode_is_red(w)) {
                rbnode_black(w);
                rbnode_red(temp->parent);
                rbtree_left_rotate(tree, temp->parent);
                w = temp->parent->right;
            }

            if (rbnode_is_black(w->left) && rbnode_is_black(w->right)) {
                rbnode_red(w);
                temp = temp->parent;

            } else {
                if (rbnode_is_black(w->right)) {
                    rbnode_black(w->left);
                    rbnode_red(w);
                    rbtree_right_rotate(tree, w);
                    w = temp->parent->right;
                }

                rbnode_copy_color(w, temp->parent);
                rbnode_black(temp->parent);
                rbnode_black(w->right);
                rbtree_left_rotate(tree, temp->parent);
                temp = tree->root;
            }

        } else {
            w = temp->parent->left;

            if (rbnode_is_red(w)) {
                rbnode_black(w);
                rbnode_red(temp->parent);
                rbtree_right_rotate(tree, temp->parent);
                w = temp->parent->left;
            }

            if (rbnode_is_black(w->left) && rbnode_is_black(w->right)) {
                rbnode_red(w);
                temp = temp->parent;

            } else {
                if (rbnode_is_black(w->left)) {
                    rbnode_black(w->right);
                    rbnode_red(w);
                    rbtree_left_rotate(tree, w);
                    w = temp->parent->left;
                }

                rbnode_copy_color(w, temp->parent);
                rbnode_black(temp->parent);
                rbnode_black(w->left);
                rbtree_right_rotate(tree, temp->parent);
                temp = tree->root;
            }
        }
    }

    rbnode_black(temp);
    rbtree_init_sentinel(sentinel);
}

int
rbtree_delete2(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *subst = NULL;
    rbtree_node_t *temp = NULL;
    rbtree_node_t *sentinel = tree->sentinel;
    rbtree_node_t *w = NULL;
    rbcolor_t red;

    if (node == NULL || node == sentinel) {
        return RBTREE_ERROR;
    }
    
    // node至多只有一个儿子
    if (node->left == sentinel || node->right == sentinel) {
        /* subst has a NIL node as a child */
        subst = node;
    } else {
        // node 有两个儿子，找子树里面只有至多一个儿子的节点
        subst = rbtree_min(node->right, sentinel);
    }

    // subst 现在最多只有一个儿子
    if (subst->left != sentinel) {
        temp = subst->left;
    } else {
        temp = subst->right;
    }

    // 接下来要用subst替换到node，因此这里重新给temp找父节点
    // 注意：temp 有可能是 sentinel
    temp->parent = subst->parent;

    // 不是根节点
    if (subst->parent) {
        // 用x代替y的位置
        if (subst == subst->parent->left) {
            subst->parent->left = temp;
        } else {
            subst->parent->right = temp;
        }
    } else {
        // 是根节点，且subst == node，且node只有最多一个儿子
        tree->root = temp;
    }

    // 保存y的颜色，因为后面可能会改
    red = rbnode_is_red(subst);
    // 如果y不是我们要删除的节点，则将其拼接以代替该节点。
    // 条件成立说明：node有两个儿子
    if (subst != node) {
        // 指回来，否二temp会丢失父节点
        if (subst->parent == node) {
            temp->parent = subst;
        } 

        /* Update subst */
        subst->parent = node->parent;
        subst->left = node->left;
        subst->right = node->right;
        rbnode_copy_color(subst, node); // 这里会改变y的颜色，所以上面要保存

        // subst已经替换了node，接下来把原来指向node的节点指向subst
        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }
        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }

        // subst 已经替换了 node，因此有可能成为根节点，这里要判断
        if (subst->parent) {
            if (node == subst->parent->left) {
                subst->parent->left = subst;
            } else {
                subst->parent->right = subst;
            }
        } else {
            tree->root = subst;
        }
    }

    // 把树的大小 -1
    rbtree_decreace(tree);

    if (red)
    {
        printf("subst is red, quick return....\n");
        return RBTREE_OK;
    }

    if (temp == sentinel) {
        printf("warning: temp == sentinel!!!!!!!!!\n");
    }

    // temp 和 subst 都是黑色，那就删除了一个黑色节点，因此要进行下列调整
    while (temp != tree->root && rbnode_is_black(temp)) {

        // 这里left和right是镜像的
        if (temp == temp->parent->left) {
            w = temp->parent->right;        // w是temp的兄弟节点

            // 兄弟节点是红色，意味着父节点是黑色
            // 对应中文wiki的情形2
            if (rbnode_is_red(w)) {
                rbnode_black(w);
                rbnode_red(temp->parent);
                rbtree_left_rotate(tree, temp->parent);
                w = temp->parent->right;
            }

            // 对应中文wiki的情形3/4，重绘 w 为红色
            if (rbnode_is_black(w->left) && rbnode_is_black(w->right)) {
                rbnode_red(w);
                temp = temp->parent;
            } else {
                // 对应中文wiki的情形5
                if (rbnode_is_black(w->right)) {
                    rbnode_black(w->left);
                    rbnode_red(w);
                    rbtree_right_rotate(tree, w);
                    w = temp->parent->right;
                }

                // 对应中文wiki的情形6
                rbnode_copy_color(w, temp->parent);
                rbnode_black(temp->parent);
                rbnode_black(w->right);
                rbtree_left_rotate(tree, temp->parent);
                temp = tree->root;
            }
        } else {
            w = temp->parent->left;

            if (rbnode_is_red(w)) {
                rbnode_black(w);
                rbnode_red(temp->parent);
                rbtree_right_rotate(tree, temp->parent);
                w = temp->parent->left;
            }

            if (rbnode_is_black(w->right) && rbnode_is_black(w->left)) {
                rbnode_red(w);
                temp = temp->parent;
            } else {
                if (rbnode_is_black(w->left)) {
                    rbnode_black(w->right);
                    rbnode_red(w);
                    rbtree_left_rotate(tree, w);
                    w = temp->parent->left;
                }
                
                rbnode_copy_color(w, temp->parent);
                rbnode_black(temp->parent);
                rbnode_black(w->left);
                rbtree_right_rotate(tree, temp->parent);
                temp = tree->root;
            }
        }
    }

    if (rbnode_is_red(temp)) {
        rbnode_black(temp);
    }

    // 有可能修改了sentinel，这里恢复一下
    rbtree_init_sentinel(sentinel);
    
    return RBTREE_OK;
}


static void
do_rbtree_inorder_traversal(rbtree_node_t *root, 
                            traversal_func func, 
                            rbtree_node_t *sentinel)
{
    if (root->left) do_rbtree_inorder_traversal(root->left, func, sentinel);
    func(root, sentinel);
    if (root->right) do_rbtree_inorder_traversal(root->right, func, sentinel);
}

void
rbtree_inorder_traversal(rbtree_t *tree, traversal_func func)
{
    do_rbtree_inorder_traversal(tree->root, func, tree->sentinel);
}


void
rbtree_layer_traversal(rbtree_t *tree, traversal_func func)
{
    int queue_size = tree->size >> 1;
    rbtree_node_t **queue1 = (rbtree_node_t **)malloc(sizeof(rbtree_node_t*) * queue_size);
    rbtree_node_t **queue2 = (rbtree_node_t **)malloc(sizeof(rbtree_node_t*) * queue_size);
    if (!queue1 || !queue2) {
        printf("malloc queue failed!\n");
        return;
    }

    rbtree_node_t **temp = NULL;
    int queue1_cur_size = 0;
    int queue2_cur_size = 0;
    rbtree_node_t *node = tree->root;
    queue1[queue1_cur_size++] = node;

    while(queue1_cur_size > 0)
    {
        for (int i = 0; i < queue1_cur_size; i++)
        {
            func(queue1[i], tree->sentinel);
            if (queue1[i]->left != NULL) {
                queue2[queue2_cur_size++] = queue1[i]->left;
            }

            if (queue1[i]->right != NULL) {
                queue2[queue2_cur_size++] = queue1[i]->right;
            }
        }

        printf("\n");

        queue1_cur_size = queue2_cur_size;
        queue2_cur_size = 0;
        temp = queue1;
        queue1 = queue2;
        queue2 = temp;
    }
}

