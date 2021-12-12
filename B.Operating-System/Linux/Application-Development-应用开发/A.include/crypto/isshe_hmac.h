
#ifndef _ISSHE_HMAC_H_
#define _ISSHE_HMAC_H_

#include <stdint.h>

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c
#define HMAC_PADKEY_LEN 64

#define HMAC_MD5_DIGEST_LEN 16
#define HMAC_SHA256_DIGEST_LEN 32

void isshe_hmac_md5(uint8_t* text, uint64_t text_len, 
    uint8_t* key, uint64_t key_len, uint8_t digest[HMAC_MD5_DIGEST_LEN]);

void isshe_hmac_sha256(unsigned char* text, int text_len, 
    unsigned char* key, int key_len, uint8_t digest[HMAC_SHA256_DIGEST_LEN]);

#endif