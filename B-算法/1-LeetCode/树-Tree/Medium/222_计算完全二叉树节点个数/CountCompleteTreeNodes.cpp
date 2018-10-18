/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : CountCompleteTreeNodes.cpp
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
    int countNodes(TreeNode* root) {
        if (root == 0)
            return 0;
        
        int k = 0;
        int leftHeight = 0;
        int rightHeight = 0;
        int sum = 0;
        
        //获取两边子树高度
        leftHeight = getleftHeight(root);
        rightHeight = getrightHeight(root);
        
        //如果两边子树高度相等，以当前root为根节点的二叉树是满二叉树，直接用公式计算
        //否则，把当前二叉树分为左右子树分别计算。
        if (leftHeight == rightHeight)
        {
            sum = (1 << leftHeight) -1;
            return sum;
        }
        else            
        {
            sum = 1 + countNodes(root->left) + countNodes(root->right);
        }
        
        return sum;
    }
    
    int getleftHeight(TreeNode *root)
    {
        int height = 0;
        while(root != NULL)
        {
            height++;
            root = root->left;
        }
        return height;
    }
    int getrightHeight(TreeNode *root)
    {
        int height = 0;
        while(root != NULL)
        {
            height++;
            root = root->right;
        }
        return height;
    }
};
