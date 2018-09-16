#include <stdio.h>

void dtox(int dnum, int jz, char *xnum, int xnum_len) 
{
    if (!xnum) {
        return ;
    }

    int i = 0;
    int j = 0;
    while(dnum >= jz && j < xnum_len - 1) {     // -1 为了留个位置给'\0'
        if (dnum % jz <= 9) {
            xnum[j++] = dnum % jz + '0';         // ‘0’ = 48
        } else {
            xnum[j++] = dnum %jz + 'A';
        }
        dnum = dnum / jz;
    }

    if (j < xnum_len - 1) {
        if (dnum % jz <= 9) {
            xnum[j++] = dnum % jz + '0';         // ‘0’ = 48
        } else {
            xnum[j++] = dnum %jz + 'A';
        }
    }

    xnum[j] = '\0';

    // 翻转
    for (int x = 0, y = j - 1; x < y; x++, y--) {
        int temp = xnum[x];
        xnum[x] = xnum[y];
        xnum[y] = temp;
    }

    printf("%s", xnum);
}

int main(void)
{
    char num[1024] = {0};
    dtox(1024, 26, num, 1024);
    return 0;
}