/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : SymmetricTree.cpp
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
    bool isSymmetric(TreeNode* root) {
        return traversal(root, root);
    }
    
    bool traversal(TreeNode *pleft, TreeNode *pright)
    {
        if (pleft == NULL && pright == NULL)
        {
            return true;
        }
        if (pleft == NULL || pright == NULL)
        {
            return false;
        }
        
        if (pleft->val != pright->val)
        {
            return false;
        }
        
        bool res1 = traversal(pleft->left, pright->right);
        bool res2 = traversal(pleft->right, pright->left);
        
        return res1 && res2;
    }
};
