/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.cpp
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
#include <iostream>
#include <vector>
using namespace std;

    string simbol[8] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wvyz"};
 
    void recursive(string &digits, int location, vector<string> &res, string &aidStr)
    {
        if (digits.size() == location)
        {
            res.push_back(aidStr);
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

           
        string aidStr;
        recursive(digits, 0, res, aidStr);
        return res;
    }
    


int main(void)
{
    vector<string> res;
    string pa = "23";
    res = letterCombinations(pa);
    for (int i = 0; i < res.size(); i++)
    {
         cout << res[i] << endl;
    }
    return 0;
}
