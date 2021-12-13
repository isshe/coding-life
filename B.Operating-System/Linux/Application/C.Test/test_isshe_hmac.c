#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "isshe_hmac.h"

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
    uint8_t result[1024];
    uint8_t data[1024] = "123";
    uint32_t data_len = 3;
    uint8_t key[1024] = "abc";
    uint32_t key_len = 3;

    uint32_t res_len = 16;
    printf("data = %u:%s\n", data_len, data);
    isshe_hmac_md5(data, data_len, key, key_len, result);
    printf("result = %s\n", result);
    print_hex_buf(result, res_len);

    res_len = 32;
    printf("data = %u:%s\n", data_len, data);
    isshe_hmac_sha256(data, data_len, key, key_len, result);
    printf("result = %s\n", result);
    print_hex_buf(result, res_len);
}
