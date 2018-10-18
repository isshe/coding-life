/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : HappyNumber.cpp
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
    bool isHappy(int n) {
        if (n <= 0)
        {
            return false;
        }
        
        string numStr = "";
        int i = 0;
        int temp = 0;
        int len = 0;
        while((len = getStr(numStr, n)) > 1)
        {
            n = 0;
            for (i = 0; i < len; i++)
            {
                temp = numStr[i] - '0';
                n += temp * temp;
            }
        }
        
        if (len == 1 && (numStr[0] == '1' || numStr[0] == '7'))
        {
            return true;
        }
        
        return false;
    }
    
    int getStr(string &str, int n)
    {
        str = "";
        char ch = 0;
        while(n > 0)
        {
            ch = n % 10 + '0';
            str = ch + str;
            n /= 10;
        }
        
        return str.size();
    }
};
