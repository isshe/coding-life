/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ExcelSheetColumnTitle.cpp
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
    string convertToTitle(int n) {
        string res = "";
        if (n < 0)
        {
            return res;
        }
        int i = 0;
        char ch = 0;
        while(n > 0)
        {
            ch = (n-1) % 26 + 'A';
            res = ch + res;
            n = (n-1) / 26;
        }
        return res;
    }
};
