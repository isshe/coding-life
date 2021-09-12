/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : valid_anagram_v2.0.0.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/09/11
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include <stdio.h>
#include <string.h>


int test(char *s, char *t);

int main(void)
{
     char s[26] = "car";//"anagram";
     char t[26] = "rat";//"nagaram";

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
     int num_s[30];
     int num_t[30];
     
     int i = 0;
     int j = 0;
     int flag = 0;
     
     for(; s[i] != '\0'; i++)
     {
          printf("s[i] - 'a' = %d\n", s[i] - 'a');
          num_s[s[i] - 'a']++;
     }
     num_s[26] = '\0';
     
     for (; t[j] != '\0'; j++)
     {
          num_t[t[j] - 'a']++;
     }
     num_t[26] = '\0';
     
     for (i = 0; num_s[i] == num_t[i]; i++)
     {
          
     }
     
     if (i == 26 && num_s[i-1] == '\0')
     {
          return 1;
     }
     else
     {
        return 0;
     }


//     return 1;
}
