/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : FindMinimumInRotatedSortedArray.cpp
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
    int findMin(vector<int>& nums) {
        
        int mid = 0;
        int i = 0;
        int j = nums.size()-1;
        
        while(i < j)
        {
            mid = (i + j) / 2;
            if (nums[mid] >= nums[i])
            {
                if (nums[i] >nums [j])
                {
                    i = mid+1;
                }
                else
                {
                    j = mid;
                }
            }
            else
            {
                j = mid;
            }
        }
        
        return nums[i];
    }
};
