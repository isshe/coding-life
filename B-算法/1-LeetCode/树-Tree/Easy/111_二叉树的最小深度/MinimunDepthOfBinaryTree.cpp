/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MinimunDepthOfBinaryTree.cpp
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
    int flag ;
    int minDepth(TreeNode* root) {
        if (root == NULL)
        {
            return 0;
        }
        
        flag = 0;
        int min = 0;
        int hight = 0;
        recursive(root, hight, min);
        
        return min;
    }
    
    void recursive(TreeNode *root, int hight, int &min)
    {
        if (root == NULL)
        {
            return;
        }
        
        hight++;
        if (root->left == NULL && root->right == NULL)
        {
            if (flag == 0)
            {
                min = hight;
                flag = 1;
            }
            else if (min > hight && flag == 1)
            {
                min = hight;
            }
        }
        
        recursive(root->left, hight, min);
        recursive(root->right, hight, min);
//        hight--;
    }
};
