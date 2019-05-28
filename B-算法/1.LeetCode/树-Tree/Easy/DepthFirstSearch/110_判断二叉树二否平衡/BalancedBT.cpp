/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BalancedBT.cpp
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
    bool isBalanced(TreeNode* root) {
        int height = 0;
        
        return recursive(root, height);
    }
    
    bool recursive(TreeNode *root, int &height)
    {
        if (root == NULL)
        {
            height = 0;
            return true;
        }
        bool resLeft = true;
        bool resRight = true;
        int leftheight = 0;
        int rightheight = 0;
        if (root->left != NULL)
        {
            resLeft = recursive(root->left, leftheight);
            leftheight++;
        }
        if (root->right != NULL)
        {
            resRight = recursive(root->right, rightheight);
            rightheight++;
        }
        
        if (leftheight - rightheight > 1 || leftheight - rightheight < -1)
        {
            return false;
        }
        
        height = leftheight > rightheight ? leftheight : rightheight;
        return resLeft && resRight;
    }
};
