解法一:
(个人感觉并不好, 要再想更好的方法)
思路:
    ﻿求出字符串长度(受strlen()影响, 第一个while()本来写的是strlen()),
    ﻿然后从字符串尾部向前数;
    ﻿从遇到第一个是字母开始计数, 到遇到第一个不是字母的, 跳出循环, 返回.
int len(char *s)
{
     int flag = 0;
     int i = 0;
     int n = 0;
     char *temp = s;
     while(*temp++ != '\0')
     {
          i++;
          printf("i = %d\n", i);
     }
     for (i = i-1; i >= 0 ; i--) //&& s[i] != ' '; i--)
     {
          if ((s[i] >= 'a' && s[i] <= 'z')
              || (s[i] >= 'A' && s[i] <= 'Z'))
          {
               flag = 1;
               n++;
               printf("if中 n = %d\n", n);
          }
          else
          {
              if (flag == 1)
              {
                   break;
              }
              continue;
          }
          printf("for n = %d\n", n);
     }
     return n;
}
注意:
遇到的问题或错误:
1. 特殊情况 空字符串("") 没有处理! //没想到这个情况, 知道后, 居然不知道空字符串代表什么, 其实空字符串就是"\0"!
2. 对于特殊情况 "_a_", "**"(*代表其他非字母空格字符) 没有处理 ;     ﻿//也遇到处理错误的情况, 
3. 原先第七行写的是 << while(*s++ != '\0') >>, 问题是, s被改变了, 但是后面还用到s!!!
4. 没有用14行的if判断是否是字母; //不知道判断是否是字母的库函数是否可以用, 没有试
5.


感悟:
    ﻿遇到问题要思考, 不能一味的用"试"来接近答案!
    ﻿能力还十分十分欠缺, 要继续努力
