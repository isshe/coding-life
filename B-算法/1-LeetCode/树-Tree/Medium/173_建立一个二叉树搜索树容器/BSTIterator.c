/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BSTIterator.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/20
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
struct BSTIterator {
    struct TreeNode *node;
    struct BSTIterator *next;
};

struct BSTIterator *bstIteratorCreate(struct TreeNode *root) {
    struct BSTIterator *newNode = NULL;
    struct BSTIterator *head = NULL;
    
    head = (struct BSTIterator *)malloc(sizeof(struct BSTIterator));
    if (head == NULL)
        return NULL;
    head->node = NULL;
    head->next = NULL;
    
    for (;root != NULL; root = root->left)
    {
        newNode = (struct BSTIterator *)malloc(sizeof(struct BSTIterator));
        if (newNode == NULL)
            return NULL;
        newNode->node = root;
        newNode->next = head->next;
        head->next= newNode;
        
    }
    
    return head;
    
}

/** @return whether we have a next smallest number */
bool bstIteratorHasNext(struct BSTIterator *iter) {
    return (iter->next != NULL);
}

/** @return the next smallest number */
int bstIteratorNext(struct BSTIterator *iter) {
    int val = iter->next->node->val;
    struct BSTIterator *freeNode = iter->next;
    struct BSTIterator *newNode = NULL;
    struct TreeNode *pNode = freeNode->node->right;     //freeNode是等下要free的节点,freeNode->node不会为NULL;
    
    //指向下一个节点，这次用过的节点会被free
    iter->next = freeNode->next;              //注意
    if (pNode != NULL)
    {
        for (; pNode != NULL; pNode = pNode->left)
        {
            newNode = (struct BSTIterator *)malloc(sizeof(struct BSTIterator));
            if (newNode == NULL)
                exit (-1);  
            newNode->node = pNode;
            newNode->next = iter->next;
            iter->next = newNode;
        }
    }
    
    free(freeNode);
    return val;
    
}

/** Deallocates memory previously allocated for the iterator */
void bstIteratorFree(struct BSTIterator *iter) {
    struct BSTIterator *freeNode = NULL;
    while(iter->next != NULL)
    {
        freeNode = iter->next;
        iter->next = freeNode->next;
        free(freeNode);
    }
}

/**
 * Your BSTIterator will be called like this:
 * struct BSTIterator *i = bstIteratorCreate(root);
 * while (bstIteratorHasNext(i)) printf("%d\n", bstIteratorNext(i));
 * bstIteratorFree(i);
 */
