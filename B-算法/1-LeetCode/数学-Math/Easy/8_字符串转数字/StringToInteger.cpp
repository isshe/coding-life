/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : StringToInteger.cpp
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
    int myAtoi(string str) {
        if(str.size() == 0)
            return 0;
            
        char ch;
        int temp = 0;
        long long res = 0;
        int i = 0;
        int flag = 1;
        int count = 0;
        int max = 0x7fffffff;
        int min = 0x80000000;    
        
        while(str[i] == ' ')
        {
            i++;
        }
        while(str[i] == '+' || str[i] == '-'  || str[i] == '0')
        {
            if (str[i] == '-')
            {
                flag = -1;
            }
            if (str[i] == '+' || str[i] == '-')
            {
                count++;
                if (count > 1)
                    return 0;
            }
            i++;
        }
        
        res = 0;
        for (; i < str.size(); i++)
        {
            ch = str[i];
            if (ch >= '0' && ch <= '9')
            {
                temp = ch - '0';
                res = res * 10 + flag * temp;
            }
            else
            {
                break;
            }
            
            if (res > max)
            {
                res = max;
                break;
            }
            else if (res < min)
            {
                res = min;
                break;
            }
        }
        
        return res;
    }
};
