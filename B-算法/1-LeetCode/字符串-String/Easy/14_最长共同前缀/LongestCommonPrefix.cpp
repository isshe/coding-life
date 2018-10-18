/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : LongestCommonPrefix.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/15
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
    string longestCommonPrefix(vector<string>& strs) {
        string res = "";
        if (strs.size() == 0)
        {
            return res;
        }
        
        int row = strs.size();
        int i = 0;
        int j = 0;
        for (i = 0; i < strs[0].size(); i++)                    //这个判断条件有点捉急，需要改进次算法
        {
            for (j = 0; strs[j][i] != '\0' && j < row - 1; j++)
            {
                if (strs[j][i] != strs[j+1][i])
                {
                    break;
                }
            }
            if (strs[j][i] == '\0' || j < row - 1)
            {
                break;
            }
            else
            {
                res.push_back(strs[j][i]);
            }
        }
        
        return res;
    }
};
