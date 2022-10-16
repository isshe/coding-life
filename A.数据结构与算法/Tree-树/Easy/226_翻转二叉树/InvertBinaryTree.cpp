/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : InvertBinaryTree.cpp
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
    TreeNode* invertTree(TreeNode* root) {
        if (root == NULL)
        {
            return root;
        }
        
        TreeNode *ptemp = root->left;
        root->left = root->right;
        root->right = ptemp;
        
        invertTree(root->left);
        invertTree(root->right);
        
        return root;
    }
};
