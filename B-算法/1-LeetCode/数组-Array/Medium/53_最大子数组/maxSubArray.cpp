/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : maxSubArray.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/26
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
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 0)
            return 0;
        int maxSum = nums[0];
        int sum = 0;
        
        int i = 0;
        int len = nums.size();
        for (i = 0; i < len; i++)
        {
            sum += nums[i];
            if (sum > maxSum)
            {
                maxSum = sum;
            }
            if (sum < 0)
            {
                sum = 0;
                continue;
            }
        }
        
        return maxSum;
    }
};
