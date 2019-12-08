

#ifndef _ISSHE_AES_CFB128_H_
#define _ISSHE_AES_CFB128_H_

#include "isshe_aes.h"

typedef void (*block128_f) (const unsigned char in[16],
                            unsigned char out[16], const void *key);


void isshe_aes_cfb128_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const isshe_aes_key_t *key,
                        unsigned char *ivec, int *num, const int enc);

#endif