#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "isshe_rand.h"

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
    print_hex_buf((uint8_t *)buf, sizeof(buf));
    isshe_rand_bytes((unsigned char *)buf, sizeof(buf));
    printf("buf = %s\n", buf);
    print_hex_buf((uint8_t *)buf, sizeof(buf));
}
