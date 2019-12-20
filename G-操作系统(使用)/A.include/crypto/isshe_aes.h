#ifndef _ISSHE_AES_H_
#define _ISSHE_AES_H_

#include <stdint.h>

//#define FULL_UNROLL 1
//#define OPENSSL_SMALL_FOOTPRINT 1
//#define STRICT_ALIGNMENT 1

#define ISSHE_AES_ENCRYPT           1
#define ISSHE_AES_DECRYPT           0
#define ISSHE_AES_MAX_ROUND         14
#define ISSHE_AES_BLOCK_SIZE        16
#define ISSHE_AES_BLOCK_SIZE_BIT    128

#define GETU32(pt) (((uint32_t)(pt)[0] << 24) ^ ((uint32_t)(pt)[1] << 16) ^ ((uint32_t)(pt)[2] <<  8) ^ ((uint32_t)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (uint8_t)((st) >> 24); (ct)[1] = (uint8_t)((st) >> 16); (ct)[2] = (uint8_t)((st) >>  8); (ct)[3] = (uint8_t)(st); }


typedef struct isshe_aes_key_s {
    int rounds;
    uint32_t rd_key[sizeof(uint32_t) * (ISSHE_AES_MAX_ROUND + 1)];
} isshe_aes_key_t;


int isshe_aes_set_encrypt_key(const unsigned char *userKey, const int bits,
                        isshe_aes_key_t *key);
int isshe_aes_set_decrypt_key(const unsigned char *userKey, const int bits,
                        isshe_aes_key_t *key);

void isshe_aes_encrypt(const unsigned char *in, unsigned char *out,
                 const isshe_aes_key_t *key);
void isshe_aes_decrypt(const unsigned char *in, unsigned char *out,
                 const isshe_aes_key_t *key);

int isshe_aes_wrap_key(isshe_aes_key_t *key, const unsigned char *iv,
                 unsigned char *out,
                 const unsigned char *in, unsigned int inlen);
int isshe_aes_unwrap_key(isshe_aes_key_t *key, const unsigned char *iv,
                   unsigned char *out,
                   const unsigned char *in, unsigned int inlen);


#endif