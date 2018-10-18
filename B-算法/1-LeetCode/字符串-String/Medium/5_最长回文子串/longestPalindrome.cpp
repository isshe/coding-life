/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : longestPalindrome.cpp
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

    string longestPalindrome(string s) {
        if (s.size() <= 1)
            return s;
        
        int i = 0;
        int stringLen = s.size();
        int p1len = 0;
        int p2len = 0;
        int maxlen = 0;
        int len = 0;
        int start = 0;
        
        for (i = 0; i < stringLen - 1; i++)
        {
            p1len = getPalindromeLen(s, i, i);
            p2len = getPalindromeLen(s, i, i+1);
            len = p1len > p2len ? p1len : p2len;
            if (len > maxlen)
            {
                maxlen = len;
                start = i - ((maxlen-1) >> 1);          //-1是为了应对baab这种情况
            }
        }
        
        return s.substr(start, maxlen);             
    }
    
    int getPalindromeLen(string &s, int center1, int center2)
    {
        int i = center1;
        int j = center2;
        int len = s.size();
        
        while(i >= 0 && j < len && s[i] == s[j])
        {
            i--;
            j++;
        }
        i++;    //这里操作多了一轮，所以要恢复，也可以直接return (j-i-1)，但是不够这样直观
        j--;
        return (j-i+1);
    }
};
