#include <stdio.h>
#include <string.h>

#include "isshe_md5.h"

void print_hex_buf(uint8_t *buf, int num)
{
    int i;
    for (i = 0; i < num; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    char buf[16];
    char data[3] = "123";

    printf("data = %ld:%s\n", sizeof(data), data);
    isshe_md5((uint8_t *)data, sizeof(data), (uint8_t *)buf);
    printf("buf = %s\n", buf);
    print_hex_buf((uint8_t *)buf, sizeof(buf));
}
