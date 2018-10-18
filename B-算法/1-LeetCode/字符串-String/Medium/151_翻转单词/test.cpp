/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.cpp
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

 using namespace std;
 #include <iostream>
 #include <stdio.h>
 #include <string>
 
 void reverseWords(string &s);
 
 int main(void)
 {
    string s;
//    cin >> s;
//    cout << s;
//    scanf("%s", s);
    getline(cin, s);
    reverseWords(s);
    cout << "cout = " <<  s << endl;
    return 0;
 }


    void reverseWords(string &s) {
        string aidStr;
        int i = 0;
        int len = s.size();
        int flag = 0;
        int j = 0;
        for (i = 0; i < len; i++)
        {
            if (s[i] == ' ')
            {
                if (flag == 1)
                {
                    aidStr.push_back(s[i]);
                    j++;
                    flag = 0;
                }
            }
            else
            {
                aidStr.push_back(s[i]);
                j++;
                flag = 1;
            }
        }
        
        cout << "aidStr = " << aidStr << endl;
        if (aidStr[j-1] == ' ')
        {
            aidStr = aidStr.substr(0, aidStr.length()-1);
            cout << "space" << endl;
        }

        len = aidStr.size() ;
        s = aidStr;
        cout << "s cp = " << s << endl;

        int saveNext = 0;
        char ch = 0;
        for (i = 0, j= 0; j < len; )        // && s[j] != '\0'
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
        
        cout << "before all:" << s << endl;
        i = 0;
        j = s.size()-1;
        while(i < j)
        {
            ch = s[j];
            s[j] = s[i];
            s[i] = ch;
            i++;
            j--;
        }
    }
