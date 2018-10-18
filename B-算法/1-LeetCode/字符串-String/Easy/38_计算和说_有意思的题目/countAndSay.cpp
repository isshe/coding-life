/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : countAndSay.cpp
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
        string temp;
        if (n <= 0)
            return temp;        
        
        temp = "1";
        if (n == 1)
            return temp;

        string res ;
            
        int i = 0;
        for(i = 1; i < n; i++)
        {
            int j = 0;
            int count = 1;
            char ch = 0;
            for(j = 0; j < temp.size()-1; j++)
            {
                if (temp[j] == temp[j+1])
                {
                    count++;
                }
                else
                {
                    ch = count + '0';
                    res = res + ch + temp[j];
                    count = 1;
                }
            }
            ch = count + '0';
            res = res + ch + temp[j];
            temp = res;
            res = "";
        }
        
        return temp;
    }
};
