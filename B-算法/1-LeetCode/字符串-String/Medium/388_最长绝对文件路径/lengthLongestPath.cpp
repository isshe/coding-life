/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.cpp
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
 
//还未完成：
//疑问：
// "a\n\rb\n\r\rc\n\r\rddddd.e" 最后的dddd.e这个文件是哪一层的呢？
//理解错了!
class Solution {
public:
    int lengthLongestPath(string input) {
        
        //1. 如果是文件，并且\t多于栈顶的，则计算长度
        //2. 如果是文件，但\t小于栈顶的,则出栈直到栈空或者栈顶\t小于文件\t.
        //3. 如果不是文件，并且\t多于栈顶的， 则入栈
        //4. 如果不是文件，冰球\t小于栈顶的，则出栈
        int     maxlen = 0;
        int     curlen = 0;
        stack<pair<int, string> >    aidStack;
        int     countT;
        string  fileOrDir;
        int     fileFlag = 0;
        int     i = 0;
        int     len = input.size();
        
        
        for (i = 0; i < len; )
        {
            fileOrDir = "";
            countT = 0;
            fileFlag = 0;

            //计算\t的个数
            if (i < len && input[i] == '\n')
            {
                i++;
                while(input[i] == '\t' && i < len)         //计算\t的个数
                {
                    countT++;
                    i++;
                }
            }
            
            for (; input[i] != '\n' && i < len; i++)
            {
                if (input[i] == '.')
                {
                    fileFlag = 1;
                }
                fileOrDir += input[i];
            }
            
                //2. 如果是文件，但\t小于栈顶的,则出栈直到栈空或者栈顶\t小于文件\t.
                //4. 如果不是文件，冰球\t小于栈顶的，则出栈
            while(!aidStack.empty() && aidStack.top().first >= countT)
            {
                curlen -= aidStack.top().second.size();
                aidStack.pop();
            }
                
            if (1 == fileFlag)               //是文件
            {
                    //1. 如果是文件，并且\t多于栈顶的，则计算长度
                if (curlen + fileOrDir.size() > maxlen)
                {
                    maxlen = curlen + fileOrDir.size();
                }                 
            }
            else
            {
                //3. 如果不是文件，并且\t多于栈顶的， 则入栈
                aidStack.push(make_pair(countT, fileOrDir+"/"));
                curlen += fileOrDir.size() + 1;
            }
            
        }
        return maxlen;
    }
};
