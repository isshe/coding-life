/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : isPalindrome.cpp
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
    bool isPalindrome(string s) {
        if (s.size() == 0)
            return true;
            
        int len = s.size();
        int i = 0;
        int j = len - 1;
        
        while(i < j)
        {
            while(!isalnum(s[i]) && i < j)
            {
                i++;
            }
            while(!isalnum(s[j]) && i < j)
            {
                j--;
            }
            
            if (strncasecmp(&s[i], &s[j], 1) != 0)
            {
                return false;
            }
            i++;
            j--;
        }
        
        return true;
    }
};
