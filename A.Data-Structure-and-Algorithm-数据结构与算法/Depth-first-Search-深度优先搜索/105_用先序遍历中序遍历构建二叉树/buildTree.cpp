/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : buildTree.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/09/18
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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        TreeNode *root = NULL;
        if (preorder.size() != inorder.size() || preorder.size() == 0)
            return root;
            
        root = buildCore(preorder, inorder, 0, preorder.size()-1, 0, inorder.size()-1);
        
        return root;
    }
    
    TreeNode *buildCore(vector<int>& preorder, vector<int>& inorder, int prestart, int preend, int instart, int inend)
    {
        if (prestart > preend)
            return NULL;
        
        int i = 0;
        TreeNode *newNode = new TreeNode(preorder[prestart]);
        
        if (prestart == preend)
            return newNode;
            
        while(inorder[instart + i] != preorder[prestart] && instart + i <= inend)         //是否要检查构造不出的情况？
        {
            i++;
        }

        newNode->left = buildCore(preorder, inorder, prestart + 1, prestart + i, instart, instart + i - 1);
        newNode->right = buildCore(preorder, inorder, prestart + i + 1, preend, instart + i + 1, inend);
        
        return newNode;
    }
};
