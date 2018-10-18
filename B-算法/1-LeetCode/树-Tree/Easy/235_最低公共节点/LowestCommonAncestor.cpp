/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : LowestCommonAncestor.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/11
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
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == NULL || p == NULL || q == NULL)
        {
            return root;
        }
        
        TreeNode *pNodep = root;
        TreeNode *pNodeq = root;
        TreeNode *res = root;
        while(pNodep != NULL || pNodeq != NULL)
        {
            if (pNodep == pNodeq)
            {
                res = pNodep;
            }
            else
            {
                break;
            }
            
            if (pNodep->val < p->val)
            {
                pNodep = pNodep->right;
            }
            else if (pNodep->val > p->val)
            {
                pNodep = pNodep->left;
            }
            
            if (pNodeq->val < q->val)
            {
                pNodeq = pNodeq->right;
            }
            else if (pNodeq->val > q->val)
            {
                pNodeq = pNodeq->left;
            }
        }
        
        return res;
    }
};
