/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/13
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
using namespace std;
#include <iostream>
#include <stdio.h>

int isHappy(int n);
int getStr(string &str, int n);
int main(void)
{
    int n = 0;
    int res = 0;
    while(1)
    {
        printf("输入n: ");
        scanf("%d", &n);
        res = isHappy(n);
        printf("res = %d\n", res);
    }
}

int isHappy(int n) {
        if (n <= 0)
        {
            return 0;
        }

        string numStr = "";
        int i = 0;
        int temp = 0;
        int len = 0;
        len = getStr(numStr, n);
        do
        {
            printf("n = %d\n", n);
            n = 0;
            for (i = 0; i < len; i++)
            {
                temp = numStr[i] - '0';
                n += temp * temp;
            }
        }while((len = getStr(numStr, n)) > 1);

        if (len == 1 && (numStr[0] == '1' || numStr[0] == '7'))
        {
            return 1;
        }

        return 0;
    }

    int getStr(string &str, int n)
    {
        str = "";
        char ch = 0;
        while(n > 0)
        {
            ch = n % 10 + '0';
            str = ch + str;
            n /= 10;
        }

        return str.size();
    }
