/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ZigZagConversion.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/10
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

    string convert(string s, int numRows) {
        if (s.size() <= numRows || numRows <= 1 )
            return s;
        
        string tempArray[numRows];
        string res = "";
        int len = s.size();
        int i = 0;
        int j = 0;

        //下面的循環的i和j寫得有點亂。        
        while(i < len)
        {
            for (j = 0; j < numRows && i < len; j++, i++)
            {
                tempArray[j] += s[i];
            }
            for (j = numRows - 2; j > 0 && i < len; j--, i++)       //沒有 “ = ”
            {
                tempArray[j] += s[i];
            }
        }
        
        i = 0;
        while(i < numRows)
        {
            res += tempArray[i];
            i++;
        }
        
        return res;
    }

};
