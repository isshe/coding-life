/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ReverseWordsInAString.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/20
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
    void reverseWords(string &s) {
        string aidStr;
        int i = 0;
        int len = s.size();
        int flag = 0;
        int j = 0;
        
        //去多余空格
        for (i = 0; i < len; i++)
        {
            if (s[i] == ' ')
            {
                if (flag == 1)
                {
                    aidStr.push_back(s[i]);
                    flag = 0;
                }
            }
            else
            {
                aidStr.push_back(s[i]);
                flag = 1;
            }
        }
        
        //去结尾空格，如果有
        if (s[len-1] == ' ')
        {
            aidStr = aidStr.substr(0, aidStr.length()-1);
        }
        
        s = aidStr;
        len = s.size() ;

        int saveNext = 0;
        char ch = 0;
        for (i = 0, j= 0; j < len; )       
        {
            while(s[j] != ' ' && j <len)
            {
                j++;
            }

            if (s[j] = ' ')
            {
                saveNext = j+1;
            }
            else if (s[j] = '\0')
            {
                saveNext = j;
            }

            for (j--; i < j ; i++, j--)
            {
                ch = s[j];
                s[j] = s[i];
                s[i] = ch;
            }
            i = saveNext;
            j = saveNext;
        }

        for (i = 0, j = s.size()-1; i < j; i++, j--)
        {
            ch = s[j];
            s[j] = s[i];
            s[i] = ch;
        }
    }
};
