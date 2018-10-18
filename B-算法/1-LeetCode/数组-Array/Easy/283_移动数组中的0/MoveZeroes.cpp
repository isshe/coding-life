/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MoveZeroes.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/12
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
    void moveZeroes(vector<int>& nums) {
        if (nums.size() == 0)
        {
            return ;
        }
        
        int i = 0;
        int j = 0;
        int temp = 0;
        int len = nums.size();
        for (i = 0, j = 0; i < len; i++)
        {
            if (nums[i] != 0)
            {
                nums[j] = nums[i];
                j++;
            }
        }
        for (; j < len; j++)
        {
            nums[j] = 0;
        }
    }
};
