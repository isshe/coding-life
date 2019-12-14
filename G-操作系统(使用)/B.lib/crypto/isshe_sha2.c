#include "isshe_sha2.h"
#include "isshe_common.h"


static const uint32_t Krnd[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
    0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
    0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
    0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
    0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define Ch(x, y, z) ((x & (y ^ z)) ^ z)
#define Maj(x, y, z) ((x & (y | z)) | (y & z))
#define SHR(x, n) (x >> n)
#define ROTR(x, n) ROTR32(x, n)
#define S0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define s0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define s1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define RND(a, b, c, d, e, f, g, h, k) \
    h += S1(e) + Ch(e, f, g) + k;      \
    d += h;                            \
    h += S0(a) + Maj(a, b, c);

#define RNDr(S, W, i, ii)                                                   \
    RND(S[(64 - i) % 8], S[(65 - i) % 8], S[(66 - i) % 8], S[(67 - i) % 8], \
        S[(68 - i) % 8], S[(69 - i) % 8], S[(70 - i) % 8], S[(71 - i) % 8], \
        W[i + ii] + Krnd[i + ii])

#define MSCH(W, ii, i) \
    W[i + ii + 16] =   \
        s1(W[i + ii + 14]) + W[i + ii + 9] + s0(W[i + ii + 1]) + W[i + ii]


static void
isshe_sha256_transform(uint32_t state[8], 
    const uint8_t block[64], uint32_t W[64], uint32_t S[8])
{
    int i;
    int j;

    be32dec_vect(W, block, 64);
    memcpy(S, state, 32);
    for (i = 0; i < 64; i += 16) {
        for (j = 0; j < 16; j++) {
            RNDr(S, W, j, i);
        }
        
        if (i == 48) {
            break;
        }
        for (j = 0; j < 16; j++) {
            MSCH(W, j, i);
        }
    }
    for (i = 0; i < 8; i++) {
        state[i] += S[i];
    }
}

void isshe_sha256_init(isshe_sha256_state_s *state)
{
    memset(ctx, 0, sizeof(*ctx));
    state->state[0] = 0x6a09e667UL;
    state->state[1] = 0xbb67ae85UL;
    state->state[2] = 0x3c6ef372UL;
    state->state[3] = 0xa54ff53aUL;
    state->state[4] = 0x510e527fUL;
    state->state[5] = 0x9b05688cUL;
    state->state[6] = 0x1f83d9abUL;
    state->state[7] = 0x5be0cd19UL;
    state->count = 0;
    //c->md_len = ISSHE_SHA256_DIGEST_LENGTH;
}

int isshe_sha256_update(isshe_sha256_state_s *c, const void *data, uint32_t len)
{
    uint32_t tmp32[ISSHE_SHA256_CBLOCK + 8];
    uint64_t i;
    uint64_t r;

    if (inlen <= 0U) {
        return 0;
    }
    r = (uint64_t) ((state->count >> 3) & 0x3f);

    state->count += ((uint64_t) inlen) << 3;
    if (inlen < ISSHE_SHA256_CBLOCK - r) {
        for (i = 0; i < inlen; i++) {
            state->buf[r + i] = in[i];
        }
        return 0;
    }
    for (i = 0; i < ISSHE_SHA256_CBLOCK - r; i++) {
        state->buf[r + i] = in[i];
    }
    isshe_sha256_transform(state->state, state->buf, &tmp32[0], &tmp32[ISSHE_SHA256_CBLOCK]);
    in += ISSHE_SHA256_CBLOCK - r;
    inlen -= ISSHE_SHA256_CBLOCK - r;

    while (inlen >= ISSHE_SHA256_CBLOCK) {
        isshe_sha256_transform(state->state, in, &tmp32[0], &tmp32[ISSHE_SHA256_CBLOCK]);
        in += ISSHE_SHA256_CBLOCK;
        inlen -= ISSHE_SHA256_CBLOCK;
    }
    inlen &= ISSHE_SHA256_CBLOCK - 1;
    for (i = 0; i < inlen; i++) {
        state->buf[i] = in[i];
    }
    //isshe_memzero((void *) tmp32, sizeof tmp32);

    return 0;
}

int isshe_sha256_final(unsigned char *md, isshe_sha256_state_s *c)
{

}