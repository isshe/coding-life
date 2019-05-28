/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BinaryTreeRightSideView.cpp
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
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
 //用右中序遍历
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> res;
        int reslen = 0;
        
        recursive(root, res, &reslen, 0);
        return res;
    }
    
    void recursive(TreeNode *root, vector<int> &res, int *reslen, int k)
    {
        if (root == NULL)
            return ;
            
        k++;
        if (k > *reslen)
        {
            *reslen = k;
            res.push_back(root->val);
        }
        recursive(root->right, res, reslen, k);         //k+1可以放这里
        recursive(root->left, res, reslen, k);
        //出去的时候k--
        k--;
    }
};
