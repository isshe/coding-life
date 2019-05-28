/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RemoveDuplicates.cpp
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
    int removeDuplicates(vector<int>& nums) {
        
        int len = nums.size();
        if (len <= 1)
            return len;
            
        int i = 0;
        int current = 0;
        
        for (i = 1; i < len; i++)
        {
            if (nums[current] != nums[i])
            {
                nums[++current] = nums[i];
            }
        }
        
        current++;
        return current;
    }
};
