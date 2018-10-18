/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : AddBinary.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/12
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
    string addBinary(string a, string b) {
        if (a.size() == 0)
        {
            return b;
        }
        if (b.size() == 0)
        {
            return a;
        }
        
        int i = a.size();
        int j = b.size();
        int len = i > j ? i : j;
        string res ; //
        res.resize(len, 0);                     //这个是需要的， 如果没有分配内存，结果不对。
        int temp= 0;
        int carry = 0;
        for (i = i-1, j = j - 1; i >= 0 || j >= 0; i--, j--)
        {
            if (i < 0)
            {
                temp = b[j] - '0' + carry;
            }
            else if (j < 0)
            {
                temp = a[i] - '0' + carry;
            }
            else 
            {
                temp = a[i] - '0' + b[j] - '0' + carry;
            }
            
            if (temp == 2)
            {
                carry = 1;
                res[--len] = '0';
            }
            else if (temp == 3)
            {
                carry = 1;
                res[--len] = '1';
            }
            else
            {
                carry = 0;
                res[--len] = temp + '0';
            }
        }
        if (carry == 1)
        {
            res = "1" + res;
        }

        return res;
    }
};
