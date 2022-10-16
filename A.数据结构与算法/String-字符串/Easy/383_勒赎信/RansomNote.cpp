/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RansomNote.cpp
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
    bool canConstruct(string ransomNote, string magazine) {
        int res[27];
//        vector<int> res(27);
        int i = 0;
        int j = 0;
        int lenRN = ransomNote.size();
        int lenMZ = magazine.size();
        for (i = 0; i < 26; i++)
        {
            res[i] = 0;
        }
        
        for (i = 0; i < lenMZ; i++)
        {
            j = magazine[i] - 'a';
            res[j]++;
        }
        
        for (i = 0; i < lenRN; i++)
        {
            j = ransomNote[i] - 'a';
            res[j]--;
        }
        
        for (i = 0; i < 26; i++)
        {
            if (res[i] < 0)
                return false;
        }
        
        return true;
    }
};
