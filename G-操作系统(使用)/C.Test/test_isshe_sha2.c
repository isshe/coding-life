#include <stdio.h>
#include <string.h>

#include "isshe_sha2.h"

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
    char buf[32];
    char data[3] = "123";

    printf("data = %ld:%s\n", sizeof(data), data);
    isshe_sha256((uint8_t *)data, sizeof(data), (uint8_t *)buf);
    //isshe_sha256((unsigned char *)buf, (const unsigned char *)data, sizeof(data));
    printf("buf = %s\n", buf);
    print_hex_buf((uint8_t *)buf, sizeof(buf));
}
