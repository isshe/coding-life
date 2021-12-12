/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : countAndSay_recursive.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/16
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
    string countAndSay(int n) {
        string res = "";
        if (n <= 0)
            return res;
        else if (n == 1)
        {
            res = "1";
            return res;
        }
        string aidStr = "1";
        recursive(res, aidStr, n - 1);
        
        return aidStr;
    }
    
    void recursive(string &res, string &aidStr, int n)
    {
        if (n == 0)
        {
            return;
        }
        int count = 0;
        int len = aidStr.size();
        char ch = 0;
        int i = 0;
        char firstSame = aidStr[0];
        for (i = 0; i < len; i++)
        {
            if (aidStr[i] == firstSame)
            {
                count++;
                if (i+1 < len && aidStr[i+1] != firstSame)
                {
                    ch = count + '0';
                    res = res + ch + firstSame;
                    count = 0;
                    firstSame = aidStr[i+1];
                }
            }
        }

        ch = count + '0';
        res = res + ch + firstSame;
        aidStr = res;
        res = "";
        recursive(res, aidStr, n - 1);
    }
};
