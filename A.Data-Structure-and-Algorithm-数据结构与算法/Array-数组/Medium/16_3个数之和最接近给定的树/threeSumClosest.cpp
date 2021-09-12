/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : threeSumClosest.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/21
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
    int threeSumClosest(vector<int>& nums, int target) {
        if (nums.size() <= 2)
            return target;
        
        sort(nums.begin(), nums.end());
        int res = nums[0] + nums[1] + nums[2];
        int sum = 0;
        int i = 0;
        int j = 0;
        int k = 0;
        int len = nums.size();
        
        for (i = 0; i < len-2; i++)
        {
            for (j = i+1, k = len-1; j < k; )//j++, k--)
            {
                sum = nums[i] + nums[j] + nums[k];
                if (sum == target)
                {
                    return sum;
                }
                else if (sum > target)
                {
                    k--;
                }
                else
                {
                    j++;
                }
                
                if (abs(target-sum) < abs(res-target))
                {
                    res = sum;
                }
            }
        }
        return res;
    }
};
