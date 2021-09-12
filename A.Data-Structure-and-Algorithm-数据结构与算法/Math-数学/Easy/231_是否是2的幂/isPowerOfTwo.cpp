/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : isPowerOfTwo.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/09
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
    bool isPowerOfTwo(int n) {
        if (n < 1)
            return false;
        while (n != 0)
        {
            if (n % 2 != 0 && n != 1)
            {
                return false;
            }
            n = n >> 1;
        }
        
        return true;
    }
};
