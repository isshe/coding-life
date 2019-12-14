#ifndef _ISSHE_MD5_H_
#define _ISSHE_MD5_H_

#include <stdint.h>

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    uint8_t   buffer[64];
} isshe_md5_t;


void isshe_md5_init(isshe_md5_t *ctx);
void isshe_md5_update(isshe_md5_t *ctx, const void *data, uint64_t size);
void isshe_md5_final(uint8_t result[16], isshe_md5_t *ctx);

#endif