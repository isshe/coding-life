/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : FactorialTrailingZeroes.cpp
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
    int trailingZeroes(int n) {
        int count = 0;
        while(n > 0)
        {
            n /= 5;
            count += n;
        }
        
        return count;
    }
};
