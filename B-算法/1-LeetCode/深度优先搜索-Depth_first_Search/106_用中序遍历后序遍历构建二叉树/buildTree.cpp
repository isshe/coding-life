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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        TreeNode *root = NULL;
        if (postorder.size() != inorder.size() || inorder.size() == 0)
            return root;
            
        root = buildCore(inorder, postorder,  0, inorder.size()-1, 0, postorder.size()-1);
        
        return root;
    }
    TreeNode *buildCore(vector<int>& inorder, vector<int>& postorder, int instart, int inend, int poststart, int postend)
    {
        if (poststart > postend)
            return NULL;
        
        int i = 0;
        TreeNode *newNode = new TreeNode(postorder[postend]);
        
        if (poststart == postend)
            return newNode;
            
        while(inorder[inend - i] != postorder[postend] && inend - i >= instart)         //是否要检查构造不出的情况？
        {
            i++;
        }

        newNode->left = buildCore(inorder, postorder, instart, inend - i - 1, poststart, postend - i - 1);
        newNode->right = buildCore(inorder, postorder,inend-i+1, inend, postend-i, postend-1);
        
        return newNode;
    }
    
};


