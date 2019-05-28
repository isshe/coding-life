/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PowerOfThree.cpp
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
    bool isPowerOfThree(int n) {
        if (n <= 0)
        {
            return false;
        }
        
        while(n)
        {
            if (n % 3 != 0 && n != 1)
            {
                return false;
            }
            
            n = n / 3;
        }
        
        return true;
    }
};
