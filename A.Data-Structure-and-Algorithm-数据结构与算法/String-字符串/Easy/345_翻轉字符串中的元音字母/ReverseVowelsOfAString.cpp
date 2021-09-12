/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ReverseVowelsOfAString.cpp
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
    string reverseVowels(string s) {
        int i = 0;
        int len = s.size();
        int j = 0;
        char tempch ;
        
        if (len== 0)
            return s;
            
        for (i = 0, j = len -1; i < j; i++, j--)
        {
            while(!isVowels(s[i]) && i < j)
            {
                i++;
            }
            while(!isVowels(s[j]) && i < j)
            {
                j--;
            }
            
            if (i >= j)
            {
                return s;
            }
            tempch = s[i];
            s[i] = s[j];
            s[j] = tempch;
        }
        
        return s;
    }
    
    bool isVowels(char ch)
    {
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' 
        || ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U')
        {
            return true;
        }
        
        return false;
    }
};
