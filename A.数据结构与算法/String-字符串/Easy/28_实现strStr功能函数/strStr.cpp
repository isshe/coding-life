/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : strStr.cpp
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
    int strStr(string haystack, string needle) {
        int lena = haystack.size();
        int lenb = needle.size();
        if (lenb > lena)
        {
            return -1;
        }
        if (lena == 0 || lenb == 0)
        {
            return 0;
        }

        
        int i = 0;
        int j = 0;
        for (i = 0; i <= lena - lenb; i++)
        {
            if (memcmp(&haystack[i], &needle[0], lenb) == 0)
            {
                return i;
            }
        }
        
        return -1;
    }
};
