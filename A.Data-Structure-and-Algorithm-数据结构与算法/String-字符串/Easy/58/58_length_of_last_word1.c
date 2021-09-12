/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : 58_length_of_last_word.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/09/11
* Description  : 方法一.
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

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
