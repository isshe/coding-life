/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BinaryTreeLevelOrderTraversalII.cpp
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
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        if (root == NULL)
            return res;
            
        queue<TreeNode *> aidQueue1;
        queue<TreeNode *> aidQueue2;
        vector<int> oneLine;
        int flag = 0;
        
        aidQueue1.push(root);
        while(!aidQueue1.empty() || !aidQueue2.empty())
        {
            if (flag == 0)
            {
                popAndPushNode(aidQueue1, aidQueue2, oneLine);
                if (aidQueue1.empty())
                {
                    res.push_back(oneLine);
                    oneLine.clear();
                    flag = 1;
                }
            }
            else
            {
                popAndPushNode(aidQueue2, aidQueue1, oneLine);
                if (aidQueue2.empty())
                {
                    res.push_back(oneLine);
                    oneLine.clear();
                    flag = 0;
                }
            }
        }
        
        for (int i = res.size() - 1, j = 0; j < i; i--, j++)
        {
            oneLine = res[i];
            res[i] = res[j];
            res[j] = oneLine;
        }
        
        return res;
    }
    
    void popAndPushNode(queue<TreeNode *>&queue1, queue<TreeNode *>&queue2, vector<int> &oneLine)
    {
        TreeNode *pNode = queue1.front();
        queue1.pop();
        oneLine.push_back(pNode->val);
        
        if (pNode->left != NULL)
        {
            queue2.push(pNode->left);
        }
        if (pNode->right != NULL)
        {
            queue2.push(pNode->right);
        }
    }
};
