/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MaximumDepthOfBT.cpp
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
    int maxDepth(TreeNode* root) {
        int MDepth = 0;
        
        recursive(root, 0, MDepth);
        
        return MDepth;
    }
    
    void recursive(TreeNode *root, int depth, int &MDepth)
    {
        if (root == NULL)
            return;
            
        if (++depth > MDepth)
        {
            MDepth =depth;
        }
        
        recursive(root->left, depth, MDepth);
        recursive(root->right, depth, MDepth);
        
        depth--;
    }
};
