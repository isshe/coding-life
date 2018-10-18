/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : valid_anagram.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/09/11
* Description  : 判断两个字符串是否属于回文构词
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

//错误的解法
#include <stdio.h>
#include <string.h>

int test(char *s, char *t);

int main(void)
{
     char s[20] = "anagram";
     char t[20] = "nagaram";

     if (test(s, t) == 1)
     {
         printf("true\n");
     }
     else
     {
         printf("false\n");
     }
}

int test(char *s, char *t)
{
     int i = 0;
     int j = 0;
     int flag = 0;

     for (; s[j] != '\0'; j++)
     {
         flag = 0;
         for (i = 0; t[i] != '\0'; i++)
         {
             if (s[j] == t[i])
             {
                 flag = 1;
                 t[i] = '+';
                 break;
             }
         }

         if (t[i] == '\0' && flag == 0)
         {
              return 0;
         }
     }
 //    printf("s= %s\n", s);

     return 1;
}
