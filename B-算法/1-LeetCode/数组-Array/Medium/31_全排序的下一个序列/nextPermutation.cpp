/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : nextPermutation.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/23
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
    void nextPermutation(vector<int>& nums) {
        if (nums.size() <= 1)
            return ;
        int i = 0;
        int j = 0;
        
        //从后往前找升序
        for (i = nums.size()-2; i >= 0; i--)
        {
            if (nums[i] < nums[i+1])
            {
                break;
            }
        }
        //从后往前找比[j]大的
        for (j = nums.size()-1; j > i; j--)
        {
            if (nums[i] < nums[j])
            {
                break;
            }
        }
        if (i >= 0)
        {
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
        }
        
        //翻转j后面的
        reverse(nums.begin()+i+1, nums.end());
    }
};
