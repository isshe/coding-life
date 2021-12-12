/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : DivideTwoIntegers.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/19
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
    int divide(int dividend, int divisor) {
        int int_min = 1 << (sizeof(int) * 8 - 1);
        int int_max = int_min - 1;
        if (divisor == 0)
        {
            return int_max ;
        }
        
        int flag = 0;
        long long longdend = (long)dividend;
        long long longdsor = (long)divisor;
        int count = 0;
        long long res = 0;
        
        if (dividend < 0)                   //记录正负号也可以用 异或再移位。(a^b) >> 31 == 1? 
        {
            longdend = -longdend;
            flag++;
        }
        if (divisor < 0)
        {
            longdsor = -longdsor;
            flag++;
        }
        
        while(longdend > (longdsor << 1))       //不要等号，把相等的情况放下面处理
        {
            count++;
            longdsor = longdsor << 1;
        }
        
        if (longdend == longdsor << 1)          //这里要注意!!!
            res = 1;
        
        
        while(count >= 0)                    //
        {
            if (longdend >= longdsor)
            {
                longdend -= longdsor;
                res += 1 << count;          //这里注意是1 << count
            }
            count--;
            longdsor >>= 1;
        }
        
        if (flag == 1)
        {
            return -res;
        }
        
        if (res > int_max)
        {
            res = int_max;
        }
        return res;
    }
};
