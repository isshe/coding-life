/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : KthSmallestElementInABST.cpp
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
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        return recursive(root, &k);
    }
    
    int recursive(TreeNode *root, int *k)
    {
        if (root == NULL)
            return 0;
        
        int val = 0;
        val = recursive(root->left, k);
        if (*k > 0)                             //这个判断可以不要，但是k会一直减
        {
            (*k)--;                             //
            if (*k == 0)
            {
                val =  root->val;
            }
        }
        if (*k > 0)
        {
            val = recursive(root->right, k);
        }
        return val;
    }
};
