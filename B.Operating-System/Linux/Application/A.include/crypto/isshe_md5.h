#ifndef _ISSHE_MD5_H_
#define _ISSHE_MD5_H_

#include <stdint.h>

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    uint8_t   buffer[64];
} isshe_md5_s;


void isshe_md5_init(isshe_md5_s *ctx);
void isshe_md5_update(isshe_md5_s *ctx, const void *data, uint64_t size);
void isshe_md5_final(isshe_md5_s *ctx, uint8_t result[16]);
int isshe_md5(uint8_t *data, uint32_t len, uint8_t result[16]);

#endif