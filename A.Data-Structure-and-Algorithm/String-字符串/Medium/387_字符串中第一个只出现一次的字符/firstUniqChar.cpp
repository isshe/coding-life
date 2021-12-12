/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : firstUniqChar.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/24
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
    int firstUniqChar(string s) {
            
        int hashTable[256] = {0};
        int i = 0;
        int len = s.size();
        
        for (i = 0; i < len; i++)
        {
            hashTable[s[i]]++;
        }
        for (i = 0; i < len; i++)
        {
            if(hashTable[s[i]] == 1)
            {
                return i;
            }
        }
        return -1;
    }
};
