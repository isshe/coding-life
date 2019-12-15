#include "isshe_common.h"
#include "isshe_md5.h"
#include "isshe_hmac.h"
#include "isshe_sha2.h"


void isshe_hmac_md5(uint8_t* text, uint64_t text_len,
    uint8_t* key, uint64_t key_len,
    uint8_t digest[HMAC_MD5_DIGEST_LEN])
{
    uint8_t ipadkey[HMAC_PADKEY_LEN];
    uint8_t opadkey[HMAC_PADKEY_LEN];
    uint8_t tk[HMAC_PADKEY_LEN];
    int i;

    if (key_len > HMAC_PADKEY_LEN) {
        isshe_md5_s tmd5;
        isshe_md5_init(&tmd5);
        isshe_md5_update(&tmd5, key, key_len);
        isshe_md5_final(&tmd5, tk);

        key = tk;
        key_len = HMAC_MD5_DIGEST_LEN;    
    }

    /* start out by storing key in pads */
    memset(ipadkey, 0, sizeof(ipadkey));
    memset(opadkey, 0, sizeof(opadkey));
    memcpy(ipadkey, key, key_len);
    memcpy(opadkey, key, key_len);

    /* XOR key with ipad and opad values */
    for (i=0; i < HMAC_PADKEY_LEN; i++) {
        ipadkey[i] ^= 0x36;
        opadkey[i] ^= 0x5c;
    }

    isshe_md5_s md5;
    /*
     * perform inner MD5
     */
    isshe_md5_init(&md5);                   /* init context for 1st pass */
    isshe_md5_update(&md5, ipadkey, HMAC_PADKEY_LEN);      /* start with inner pad */
    isshe_md5_update(&md5, text, text_len); /* then text of datagram */
    isshe_md5_final(&md5, digest);          /* finish up 1st pass */
    /*
     * perform outer MD5
     */
    isshe_md5_init(&md5);                   /* init context for 2nd pass */
    isshe_md5_update(&md5, opadkey, HMAC_PADKEY_LEN);     /* start with outer pad */
    isshe_md5_update(&md5, digest, HMAC_MD5_DIGEST_LEN);     /* then results of 1st hash */
    isshe_md5_final(&md5, digest);          /* finish up 2nd pass */
}


void isshe_hmac_sha256(unsigned char* text, int text_len, 
    unsigned char* key, int key_len, 
    uint8_t digest[HMAC_SHA256_DIGEST_LEN])
{
    uint8_t ipadkey[HMAC_PADKEY_LEN];
    uint8_t opadkey[HMAC_PADKEY_LEN];
    uint8_t tk[HMAC_PADKEY_LEN];
    int i;

    if (key_len > HMAC_PADKEY_LEN) {
        isshe_sha256_state_s tsha256;
        isshe_sha256_init(&tsha256);
        isshe_sha256_update(&tsha256, key, key_len);
        isshe_sha256_final(&tsha256, tk);

        key = tk;
        key_len = HMAC_SHA256_DIGEST_LEN;    
    }

    /* start out by storing key in pads */
    memset(ipadkey, 0, sizeof(ipadkey));
    memset(opadkey, 0, sizeof(opadkey));
    memcpy(ipadkey, key, key_len);
    memcpy(opadkey, key, key_len);

    /* XOR key with ipad and opad values */
    for (i=0; i < HMAC_PADKEY_LEN; i++) {
        ipadkey[i] ^= 0x36;
        opadkey[i] ^= 0x5c;
    }

    isshe_sha256_state_s sha256;
    /*
     * perform inner SHA256
     */
    isshe_sha256_init(&sha256);                   /* init context for 1st pass */
    isshe_sha256_update(&sha256, ipadkey, HMAC_PADKEY_LEN);      /* start with inner pad */
    isshe_sha256_update(&sha256, text, text_len); /* then text of datagram */
    isshe_sha256_final(&sha256, digest);          /* finish up 1st pass */
    /*
     * perform outer SHA256
     */
    isshe_sha256_init(&sha256);                   /* init context for 2nd pass */
    isshe_sha256_update(&sha256, opadkey, HMAC_PADKEY_LEN);     /* start with outer pad */
    isshe_sha256_update(&sha256, digest, HMAC_SHA256_DIGEST_LEN);     /* then results of 1st hash */
    isshe_sha256_final(&sha256, digest);          /* finish up 2nd pass */
}