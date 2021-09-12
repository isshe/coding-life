class Solution {
public:
    void sortColors(vector<int>& nums) {
        int countRed = 0;
        int countWhite = 0;
        int countBlue = 0;
        int i = 0;
        int j = 0;
        int len = nums.size();
        
        for (i = 0; i < len; i++)
        {
            if (nums[i] == 0)
            {
                countRed++;
            }
            else if (nums[i] == 1)
            {
                countWhite++;
            }
            else 
            {
                countBlue++;
            }
        }
        for (i = 0; i < countRed; i++)
        {
            nums[i] = 0;
        }
        for (j = 0; j < countWhite; j++, i++)
        {
            nums[i] = 1;
        }
        for (j = 0; j < countBlue; j++, i++)
        {
            nums[i] = 2;
        }
    }
};
