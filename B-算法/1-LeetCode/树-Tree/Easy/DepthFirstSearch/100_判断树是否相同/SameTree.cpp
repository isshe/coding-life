/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : SameTree.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/06
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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (p == NULL && q == NULL)
        {
            return true;
        }
        
        if (p == NULL || q == NULL)
        {
            return false;
        }
        
        if (p->val != q->val)
        {
            return false;
        }
        bool resLeft = true;
        bool resRight = true;
        resLeft = isSameTree(p->left, q->left);
        resRight = isSameTree(p->right, q->right);
        
        return resLeft && resRight;
    }
};
