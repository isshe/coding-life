/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MajorityElement.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/04
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
    int majorityElement(vector<int>& nums) {
        if (nums.size() == 0)
            return 0;
            
        sort(nums.begin(), nums.end());
        return nums[nums.size() / 2];
    }
};
