#include <stdio.h>
#include <string.h>

#include "isshe_rand.h"

void print_hex_buf(char *buf, int num)
{
    int i;
    for (i = 0; i < num; i++) {
        printf("%.2x ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    char buf[16];
    print_hex_buf(buf, sizeof(buf));
    isshe_rand_bytes((unsigned char *)buf, sizeof(buf));
    printf("buf = %s\n", buf);
    print_hex_buf(buf, sizeof(buf));
}
