#include <stdio.h>
#include <string.h>

int int_pow(int x, int y)
{
    int res = 1;

    for (int i = 1; i <= y; i++) {
        res *= x;
    }

    return res;
}

int xtod(char num[], int jz)
{
    int dnum = 0;
    int b = 0;
    int len = 0;

    len = strlen(num);

    for (int i = 0; i < len; i++) {
        if (num[i] >= 'a') {
            b = num[i] - 'a' + 10;
        } else if (num[i] >= 'A') {
            b = num[i] - 'A' + 10;
        } else {
            b = num[i] - '0';
        }
        dnum += b * int_pow(jz, len - i - 1);
    }
    return dnum;
}

int main(void)
{
    printf("%d", xtod("FF", 16));
    
    return 0;
}