#include <stdio.h>

#include "isshe_rand.h"

int isshe_rand_bytes_dev_urandom(unsigned char *buf, int num)
{
    int res;
    FILE *fp = fopen(ISSHE_DEV_URANDOM, "rb");
    if (!fp) {
        return -1;
    }

    res = fread(buf, 1, num, fp);
    fclose(fp);

    if (res != num) {
        return -1;
    }

    return res;
}

int isshe_rand_bytes(unsigned char *buf, int num)
{
    return isshe_rand_bytes_dev_urandom(buf, num);
}