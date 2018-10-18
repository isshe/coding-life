/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MajorityElement.c
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
int majorityElement(int* nums, int numsSize) {
    if (nums == NULL)
        return 0;
    
    int i = 0;
    int count = 0;
    int count_num = 0;
    
    for (i = 0; i < numsSize; i++)
    {
        if (count == 0)
        {
            count++;
            count_num = nums[i];
            continue;
        }
        
        if (nums[i] == count_num)
        {
            count++;
        }
        else
        {
            count--;
        }
    }
    
    if (count > 0)
        return count_num;
    else
        return 0;
}
