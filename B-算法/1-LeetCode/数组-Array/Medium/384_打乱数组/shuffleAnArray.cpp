/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : shuffleAnArray.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/24
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
class Solution {
public:
    vector<int> resetNums;
    Solution(vector<int> nums) {
        int i = 0;
        int len = nums.size();
        for (i = 0; i < len; i++)
        {
            resetNums.push_back(nums[i]);
        }
    }
    
    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return resetNums;
    }
    
    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
        vector<int> temp(resetNums);
        int len = resetNums.size();
        int i = 0;
        int randj = 0;
        for (i = 0; i < len; i++)
        {
            randj = rand() % (len-i);           //为什么？不懂。
            swap(temp[i], temp[randj+i]);
        }
        
        return temp;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(nums);
 * vector<int> param_1 = obj.reset();
 * vector<int> param_2 = obj.shuffle();
 */
