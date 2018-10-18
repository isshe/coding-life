/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RemoveDuplicates.c
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
int removeDuplicates(int* nums, int numsSize) {

    if (numsSize <= 1)
    {
        return numsSize;
    }
    
    int i = 1;
    int current = 0;
    
    while(i < numsSize)
    {
        if (nums[current] != nums[i])           //这里多一个分号找半天
        {
            nums[++current] = nums[i];
        }
        i++;
    }
    current++;
    return current;
}
