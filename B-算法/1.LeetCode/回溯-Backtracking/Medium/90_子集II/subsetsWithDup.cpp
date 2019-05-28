class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        int len = nums.size();
        vector<vector<int>> res;
        vector<int> aid;
        
        sort(nums.begin(), nums.end());
        recursive(nums, 0, aid, res);

        return res;
    }
    
    void recursive(vector<int>&nums, int pos, vector<int>& aid, vector<vector<int>>& res)
    {
        res.push_back(aid);
        
        int i = 0;
        int len = nums.size();
        
        for (i = pos; i < len; i++)
        {
            if (i > pos &&  nums[i] == nums[i-1])   //这里改nums[pos]不可以，例如122， 1和第一个2比，再1和第二个2比，重复了。
            {
                continue;
            }
            
            aid.push_back(nums[i]);
            recursive(nums, i+1, aid, res);
            aid.pop_back();
        }
    
    }
};