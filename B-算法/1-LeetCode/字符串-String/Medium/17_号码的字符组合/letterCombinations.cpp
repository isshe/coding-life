/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : letterCombinations.cpp
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

//    void recursive(string &digits, int location, vector<string> &res, string &aidStr);
    
    string simbol[8] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    
    void recursive(string &digits, int location, vector<string> &res, string &aidStr)
    {
        if (digits.size() <= location)
        {
            res.push_back(aidStr);
            return;                                 //漏了这句出现runtime error，找了好久。。。还是不够。
        }
        string saveStr = aidStr;
        int i = 0;
        int num = digits[location] - '0' - 2;       //
        int simbolLen = simbol[num].size();
        for (i = 0; i < simbolLen; i++)
        {
            char ch = simbol[num][i];
            aidStr += ch;
            recursive(digits, location+1, res, aidStr);
            aidStr = saveStr;
        }
    }    
    
    vector<string> letterCombinations(string digits) {
        vector<string> res;
        if (digits.size() <= 0)
            return res;
           
        string aidStr = "";
        recursive(digits, 0, res, aidStr);
        return res;
    }
    

};
