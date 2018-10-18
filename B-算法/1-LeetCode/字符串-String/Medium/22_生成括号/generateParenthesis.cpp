/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : generateParenthesis.cpp
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
    vector<string> generateParenthesis(int n) {
        
        vector<string> res;
        string aidStr = "";
        recursive(res, aidStr, n, n);
        return res;
    }
    
   void recursive(vector<string> &res, string aidStr, int left, int right)
    {
        if (left == 0 && right == 0)
        {
            res.push_back(aidStr);
            return ;
        }
        if (left > 0)
        {
            recursive(res, aidStr+"(", left-1, right);
        }
        if (right > left && right > 0)
        {
            recursive(res, aidStr+")", left, right-1);
        }
    }
};
