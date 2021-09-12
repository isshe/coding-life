/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ValidateBST.cpp
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
    bool isValidBST(TreeNode* root) {
        TreeNode *beforeNode = NULL;
        return recursive(root, &beforeNode);
    }
    
    bool recursive(TreeNode *root, TreeNode **beforeNode)
    {
        if (root == NULL)
            return true;
        bool leftres = true;
        bool rightres = true;
        
        leftres = recursive(root->left, beforeNode);
        if (*beforeNode != NULL)
        {
            if ((*beforeNode)->val >=root->val)
            {
                leftres =  false;
            }
        }
        *beforeNode = root;
        if (leftres == true)
        {
            rightres = recursive(root->right, beforeNode);
        }
        return leftres && rightres;
    }
};
