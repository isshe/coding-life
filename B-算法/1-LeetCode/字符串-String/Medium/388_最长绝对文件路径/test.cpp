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
 
 #include <iostream>
 #include <string>
 #include <stack>
 #include <vector>
 using namespace std;
 /*
 int lengthLongestPath(string input) {  
        input+= '\n';  
        int len = input.size(), ans = 0, spaceCnt = 0, curPathLen =0, dotFlag = 0;  
        stack<pair<string, int> > st;  
        string curDir, extension;  
        for(int i = 0; i < len; i++)  
        {  
            if(input[i]!='\n')  
            {  
                if(input[i]!='\t') curDir += input[i];  
//                if(dotFlag) extension += input[i];  
                if(input[i]=='\t') spaceCnt++;  
                if(input[i]=='.') dotFlag = 1;  
                continue;  
            }    
            while(!st.empty() && spaceCnt <= st.top().second)  
            {  
                curPathLen -= st.top().first.size();  
                st.pop();  
            }  
            if(dotFlag) ans = max(curPathLen + (int)curDir.size() + 1, ans);  
            else  
            {  
                st.push(make_pair("/"+ curDir, spaceCnt));  
                curPathLen += curDir.size() + 1;  
            }  
            //extension = "", 
            dotFlag = 0, spaceCnt = 0, curDir = "";  
        }  
        return ans==0?0:ans-1;  
    }  
*/
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
                     cout << "pop curlen = " << curlen << endl;
                }
                
                if (1 == fileFlag)               //是文件
                {
                    //1. 如果是文件，并且\t多于栈顶的，则计算长度
                        if (curlen + fileOrDir.size() > maxlen)
                        {
                            maxlen = curlen + fileOrDir.size();
                        }                 
                    cout << "curlen + fileOrDir.size = " << curlen + fileOrDir.size() << ", fileOrDir = " << fileOrDir << endl;
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

 int main(void)
 {
    string input = "skd\n\talskjv\n\t\tlskjf\n\t\t\tklsj.slkj\n\t\tsdlfkj.sdlkjf\n\t\tslkdjf.sdfkj\n\tsldkjf\n\t\tlskdjf\n\t\t\tslkdjf.sldkjf\n\t\t\tslkjf\n\t\t\tsfdklj\n\t\t\tlskjdflk.sdkflj\n\t\t\tsdlkjfl\n\t\t\t\tlskdjf\n\t\t\t\t\tlskdjf.sdlkfj\n\t\t\t\t\tlsdkjf\n\t\t\t\t\t\tsldkfjl.sdlfkj\n\t\t\t\tsldfjlkjd\n\t\t\tsdlfjlk\n\t\t\tlsdkjf\n\t\tlsdkjfl\n\tskdjfl\n\t\tsladkfjlj\n\t\tlskjdflkjsdlfjsldjfljslkjlkjslkjslfjlskjgldfjlkfdjbljdbkjdlkjkasljfklasjdfkljaklwejrkljewkljfslkjflksjfvsafjlgjfljgklsdf.a";//"dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";//"dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";//"dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext";//
    int len = lengthLongestPath(input);
    cout << len << endl;
    return len ;
 }
 

