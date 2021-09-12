/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : intToRoman.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/23
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
    string intToRoman(int num) {
        
        string  symble[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
        int     value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        string  res ;
        
        for (int i = 0; num != 0 && i < 13; i++)
        {
            while(num >= value[i])
            {
                res += symble[i];
                num -= value[i];
            }
        }
        
        return res;
    }
};

