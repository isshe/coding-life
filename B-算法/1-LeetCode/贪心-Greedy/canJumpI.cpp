class Solution {
public:
    bool canJump(vector<int>& nums) {
        int len = nums.size();
        int i = 0;
        
        int step = nums[0];
        for (i = 1; i < len; i++)
        {
            if (step <= 0)
            {
                return false;
            }
            else
            {
                step--;
            }
            if (nums[i] > step)
            {
                step = nums[i];
            }
        }
        
        return true;
    }
};