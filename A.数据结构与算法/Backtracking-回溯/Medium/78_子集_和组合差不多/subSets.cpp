class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> aid;
        int i = 0;
        int len = nums.size();
        
        for(i = 0; i <= len; i++)
        {
            recursive(nums, res, aid, 0, i);
        }
        
        return res;
    }
    
    void recursive(vector<int> &nums, vector<vector<int>> &res, vector<int> &aid, int next, int k)
    {
        if (k == 0)
        {
            res.push_back(aid);
        }
        
        int i = 0;
        int len = nums.size();
        for (i = next; i < len; i++)
        {
            aid.push_back(nums[i]);
            recursive(nums, res, aid, i+1, k-1);
            aid.pop_back();
        }
    }
};