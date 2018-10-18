/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PathSum.cpp
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
    bool hasPathSum(TreeNode* root, int sum) {
        if (root == NULL)
            return false;
        int currentSum = 0;
        return recursive(root, sum, currentSum);
    }
    
    bool recursive(TreeNode *root, int sum, int currentSum)
    {
        if (root == NULL)
        {
            return false;        //
        }
        
        currentSum += root->val;
        if (root->left == NULL && root->right == NULL && currentSum == sum)
        {
            return true;
        }
        
        bool tempres = recursive(root->left, sum, currentSum);      //进入左节点
        if (tempres == false)
        {
            tempres = recursive(root->right, sum, currentSum);      //进入右节点
        }
        
        return tempres;
    }
};


