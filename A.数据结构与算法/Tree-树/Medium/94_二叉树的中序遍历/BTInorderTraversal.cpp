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
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        Traversal(root, res);
        return res;
    }
    
    void Traversal(TreeNode *root, vector<int> &res)
    {
        if (root == NULL)
            return;
            
        Traversal(root->left, res);
        res.push_back(root->val);
        Traversal(root->right, res);
    }
};