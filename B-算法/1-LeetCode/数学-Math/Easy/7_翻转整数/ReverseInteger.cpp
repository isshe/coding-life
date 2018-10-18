/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ReverseInteger.cpp
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
    int reverse(int x) {
        int flag = 0;
        long temp = x;
        string strNum;
        int i = 0;
        int min = 1 << (sizeof(int) * 8 - 1);       //如果int 4个字节，则右移31位。
        int max = min - 1;
        
        if (x < 0)
        {
            flag = 1;
            temp =(long)-x;
        }
        
        while(temp > 0)
        {
            strNum[i++] = temp % 10 + '0';
            temp /= 10;
        }
        
        int j = 0;
        while(j < i)
        {
            temp = temp * 10 + strNum[j] - '0';
            j++;
        }
        
        if (flag == 1)
        {
            temp = -temp;
        }
        if (temp > max || temp < min)
        {
            temp = 0;
        }
        return temp;
    }
};
