/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : UglyNumber.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/13
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
    bool isUgly(int num) {
        if (num < 1)
        {
            return false;
        }
        int tempnum = num;
        while(tempnum % 2 == 0)
        {
            tempnum /= 2;
        }
        while(tempnum % 3 == 0)
        {
            tempnum /= 3;
        }
        while(tempnum % 5 == 0)
        {
            tempnum /= 5;
        }
        
        if (tempnum == 1)
        {
            return true;
        }
        
        return false;
    }
};
