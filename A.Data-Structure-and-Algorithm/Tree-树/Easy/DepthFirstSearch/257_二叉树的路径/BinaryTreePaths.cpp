/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BinaryTreePaths.cpp
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
    vector<string> binaryTreePaths(TreeNode* root) {
        
        vector<string> res;
        string str;
        
        recursive(root, res, str);
        return res;
    }
    
    void recursive(TreeNode *root, vector<string> &res, string &str)
    {
        if (root == NULL)
         return ;
         
         //这里int 转 string有点麻烦。
         stringstream ss;
         ss << root->val;
         
         int len = str.size();      //这里保存长度，因为后面还要减去添加的字符。

        str = str + ss.str();
        if (root->left == NULL && root->right == NULL)  //根节点
        {
            res.push_back(str);
        }
        
        str = str + "->";           //这个放这里是为了如果是根节点不会运行到这里。
        recursive(root->left, res, str);
        recursive(root->right, res, str);
        
        str = str.substr(0, len);
    }
};
