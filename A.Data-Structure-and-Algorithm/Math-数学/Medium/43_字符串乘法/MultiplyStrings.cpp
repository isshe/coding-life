/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MultiplyStrings.cpp
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
    string multiply(string num1, string num2) {
        if (num1.size() == 0 || num2.size() == 0)
            return "";
        if (num1[0] == '0' || num2[0] == '0')
            return "0";
    
        int i = num1.size() - 1;
        int j = num2.size() - 1;
        int k = num1.size() + num2.size() - 2;
        string res(k+1, '0');
        int carry = 0;
        int bitmul = 0;
        int temp = 0;
        int savek = k;
        char ch = 0;
        
        for (i = num1.size() - 1; i >= 0; i--)
        {
            k = savek--;
            carry = 0;                                              //这里注意
            for (j = num2.size() - 1; j>= 0; j--)
            {
                bitmul = (num1[i]-'0') * (num2[j] - '0') + carry;
                temp = res[k] - '0' + bitmul;
                carry = temp / 10;
                res[k] = temp % 10 + '0';
                k--;
            }
            if (carry > 0)                                          //这里注意
            {
                res[k] = carry + '0';
            }
        }
        
        if (carry > 0)
        {
            ch = carry + '0';
            res = ch + res;
        }
        
        return res;
    }
};
