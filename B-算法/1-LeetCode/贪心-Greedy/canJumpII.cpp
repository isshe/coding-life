class Solution {
public:
    bool canJump(vector<int>& nums) {
        int len = nums.size();
        int i = 0;
        
        int step = 0;
        for (i = 0; i < len && i <= step; i++)
        {
            step = step > (i+nums[i]) ? step : (i + nums[i]);
        }
        
        return (i >= len);
    }
};