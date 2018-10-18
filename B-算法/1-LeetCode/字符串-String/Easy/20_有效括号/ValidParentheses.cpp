/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ValidParentheses.cpp
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
    bool isValid(string s) {
        stack<char> aidStack;
        int len = s.size();
        if (len == 0)
        {
            return false;
        }
        
        char tempch = 0;
        int i = 0;
        for (i = 0; i < len; i++)
        {
            switch(s[i])
            {
                case '(' :
                case '[' :
                case '{' : aidStack.push(s[i]); break;
                case ')' :
                    if (aidStack.empty() || aidStack.top() != '(')
                    {
                        return false;
                    }
                    aidStack.pop();
                    break;
                case ']' :
                    if (aidStack.empty() || aidStack.top() != '[')
                    {
                        return false;
                    }
                    aidStack.pop();
                    break;
                
                case '}' :
                    if (aidStack.empty() || aidStack.top() != '{')
                    {
                        return false;
                    }
                    aidStack.pop();
                    break;
                default:
                    return false;
            }
        }
        if (!aidStack.empty())
        {
            return false;
        }
        
        return true;
    }
};
