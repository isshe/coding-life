class Solution {
public:
    void sortColors(vector<int>& nums) {
        int len = nums.size();
        int whitePos = 0;
        int bluePos = len-1;
        int i = 0;
        
        for (i = 0; i < len; i++)
        {
            if (nums[i] == 0)
            {
                swap(nums[whitePos], nums[i]);
                whitePos++;
            }
            else if (nums[i] == 2 && bluePos > i)
            {
                swap(nums[i], nums[bluePos]);
                bluePos--;
                i--;
            }
        }
        
    }
};