/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : LengthOfLongestSubstring.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/22
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
    int lengthOfLongestSubstring(string s) {
        int hashTable[256] = {0};
        
        int i = 0;
        int len = s.size();
        int sublen = 0;
        int curSubStart = -1;            //当前子串开始的位置-1（从0开始遍历，所以0-1 == -1. 用结尾减开始得到子串长度）
        memset(hashTable, -1, sizeof(hashTable));
        for (i = 0; i < len; i++)
        {
            //如果当前子串中此元素已经出现过了
            if (hashTable[s[i]] > curSubStart)
            {
                curSubStart = hashTable[s[i]];
            }
            
            //哈希表对应位置存的是符号出现的位置
            //当前位置永远比以前的大，会覆盖以前的，所以保存都是最新的位置
            hashTable[s[i]] = i;
            
            //
            if (i - curSubStart > sublen)
            {
                sublen = i - curSubStart;
            }
        }
        
        return sublen;
    }
};
