/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : myPow.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/26
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
    double myPow(double x, int n) {
        double res = 0.0;
        
        if (n < 0)
        {
            if (n == INT_MIN)
            {
                return (1.0 / (myPow(x, INT_MIN-1) * x));         //相当于(x, INT_MAX) * x
            }
            return (1.0 / myPow(x,-n));
        }
        
        if (n == 0)
            return 1.0;
        res = 1.0;
        for (; n > 0; n >>= 1)
        {
            if ((n & 1) == 1)
            {
                res *= x;
            }
            x *= x;
        }
        
        return res;
    }
};
