/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : FindPeakElement.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/18
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
    int findPeakElement(vector<int>& nums) {
        if (nums.size() <= 1)
            return 0;
        
        int index = 0;
        int i = 0;
        int len = nums.size();
        
        index = 0;
        for (i = 1; i < len; i++)
        {
            if (nums[i] > nums[i-1])
            {
                index = i;
            }
            
            if ((i == len-1) || ((i < len -1 ) && nums[i+1] < nums[i]))
            {
                return index;
            }
        }
        
        return 0;
    }
};
