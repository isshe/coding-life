/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BSTIterator.cpp
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
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class BSTIterator {
public:
    stack<TreeNode *> aidStack;
    
    BSTIterator(TreeNode *root) {
        while(root != NULL)
        {
            aidStack.push(root);
            root = root->left;
        }
    }

    /** @return whether we have a next smallest number */
    bool hasNext() {
        return (!aidStack.empty());
    }

    /** @return the next smallest number */
    int next() {
        int val = 0;
        TreeNode *pNode = aidStack.top();
        aidStack.pop();
        val = pNode->val;
        if (pNode->right != NULL)
        {
            pNode = pNode->right;
            while(pNode != NULL)
            {
                aidStack.push(pNode);
                pNode = pNode->left;
            }
        }
        return val;
    }
};

/**
 * Your BSTIterator will be called like this:
 * BSTIterator i = BSTIterator(root);
 * while (i.hasNext()) cout << i.next();
 */
