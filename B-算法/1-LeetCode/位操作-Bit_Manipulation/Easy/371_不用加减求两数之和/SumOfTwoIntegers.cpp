/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : SumOfTwoIntegers.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/11
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
    int getSum(int a, int b) {
        int carry =0;
        int sum = 0;
        do
        {
            carry = (a & b) << 1;
            sum = a ^ b;            //不进位的的相加
            
            a = carry;
            b = sum;
        }
        while(carry != 0);
        
        return sum;
    }
};
