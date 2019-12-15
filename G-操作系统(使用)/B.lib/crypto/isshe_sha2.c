/*
 * Base on openssl
 */

#include "isshe_sha2.h"
#include "isshe_common.h"

/*
 * Note that FIPS180-2 discusses "Truncation of the Hash Function Output."
 * default: case below covers for it. It's not clear however if it's
 * permitted to truncate to amount of bytes not divisible by 4. I bet not,
 * but if it is, then default: case shall be extended. For reference.
 * Idea behind separate cases for pre-defined lengths is to let the
 * compiler decide if it's appropriate to unroll small loops.
 */
#define SHA256_MAKE_STRING(st,s)   do {   \
        unsigned long ll;               \
        unsigned int  nn;               \
        for (nn=0;nn<ISSHE_SHA256_DIGEST_LENGTH/4;nn++)       \
        {   ll=(st)->state[nn]; (void)HOST_l2c(ll,(s));   }  \
        break;                          \
    } while (0)


static const uint32_t K256[64] = {
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

#if defined(ISSHE_BIG_ENDIAN)

# define HOST_c2l(c,l)  (l =(((unsigned long)(*((c)++)))<<24),          \
                         l|=(((unsigned long)(*((c)++)))<<16),          \
                         l|=(((unsigned long)(*((c)++)))<< 8),          \
                         l|=(((unsigned long)(*((c)++)))    )           )
# define HOST_l2c(l,c)  (*((c)++)=(unsigned char)(((l)>>24)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>>16)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>> 8)&0xff),      \
                         *((c)++)=(unsigned char)(((l)    )&0xff),      \
                         l)

#elif defined(ISSHE_LITTLE_ENDIAN)

# define HOST_c2l(c,l)  (l =(((unsigned long)(*((c)++)))    ),          \
                         l|=(((unsigned long)(*((c)++)))<< 8),          \
                         l|=(((unsigned long)(*((c)++)))<<16),          \
                         l|=(((unsigned long)(*((c)++)))<<24)           )
# define HOST_l2c(l,c)  (*((c)++)=(unsigned char)(((l)    )&0xff),      \
                         *((c)++)=(unsigned char)(((l)>> 8)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>>16)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>>24)&0xff),      \
                         l)
#endif

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

/*
 * FIPS specification refers to right rotations, while our ROTATE macro
 * is left one. This is why you might notice that rotation coefficients
 * differ from those observed in FIPS document by 32-N...
 */
# define Sigma0(x)       (ROTATE((x),30) ^ ROTATE((x),19) ^ ROTATE((x),10))
# define Sigma1(x)       (ROTATE((x),26) ^ ROTATE((x),21) ^ ROTATE((x),7))
# define sigma0(x)       (ROTATE((x),25) ^ ROTATE((x),14) ^ ((x)>>3))
# define sigma1(x)       (ROTATE((x),15) ^ ROTATE((x),13) ^ ((x)>>10))

# define Ch(x,y,z)       (((x) & (y)) ^ ((~(x)) & (z)))
# define Maj(x,y,z)      (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#  define ROUND_00_15(i,a,b,c,d,e,f,g,h)          do {    \
        T1 += h + Sigma1(e) + Ch(e,f,g) + K256[i];      \
        h = Sigma0(a) + Maj(a,b,c);                     \
        d += T1;        h += T1;                } while (0)

#  define ROUND_16_63(i,a,b,c,d,e,f,g,h,X)        do {    \
        s0 = X[(i+1)&0x0f];     s0 = sigma0(s0);        \
        s1 = X[(i+14)&0x0f];    s1 = sigma1(s1);        \
        T1 = X[(i)&0x0f] += s0 + s1 + X[(i+9)&0x0f];    \
        ROUND_00_15(i,a,b,c,d,e,f,g,h);         } while (0)

static void sha256_block_data_order(isshe_sha256_state_s *state, const void *in,
                                    size_t num)
{
    uint32_t a, b, c, d, e, f, g, h, s0, s1, T1;
    uint32_t X[16];
    int i;
    const unsigned char *data = in;
    const union {
        long one;
        char little;
    } is_endian = {
        1
    };

    while (num--) {

        a = state->state[0];
        b = state->state[1];
        c = state->state[2];
        d = state->state[3];
        e = state->state[4];
        f = state->state[5];
        g = state->state[6];
        h = state->state[7];

        if (!is_endian.little && sizeof(uint32_t) == 4
            && ((size_t)in % 4) == 0) {
            const uint32_t *W = (const uint32_t *)data;

            T1 = X[0] = W[0];
            ROUND_00_15(0, a, b, c, d, e, f, g, h);
            T1 = X[1] = W[1];
            ROUND_00_15(1, h, a, b, c, d, e, f, g);
            T1 = X[2] = W[2];
            ROUND_00_15(2, g, h, a, b, c, d, e, f);
            T1 = X[3] = W[3];
            ROUND_00_15(3, f, g, h, a, b, c, d, e);
            T1 = X[4] = W[4];
            ROUND_00_15(4, e, f, g, h, a, b, c, d);
            T1 = X[5] = W[5];
            ROUND_00_15(5, d, e, f, g, h, a, b, c);
            T1 = X[6] = W[6];
            ROUND_00_15(6, c, d, e, f, g, h, a, b);
            T1 = X[7] = W[7];
            ROUND_00_15(7, b, c, d, e, f, g, h, a);
            T1 = X[8] = W[8];
            ROUND_00_15(8, a, b, c, d, e, f, g, h);
            T1 = X[9] = W[9];
            ROUND_00_15(9, h, a, b, c, d, e, f, g);
            T1 = X[10] = W[10];
            ROUND_00_15(10, g, h, a, b, c, d, e, f);
            T1 = X[11] = W[11];
            ROUND_00_15(11, f, g, h, a, b, c, d, e);
            T1 = X[12] = W[12];
            ROUND_00_15(12, e, f, g, h, a, b, c, d);
            T1 = X[13] = W[13];
            ROUND_00_15(13, d, e, f, g, h, a, b, c);
            T1 = X[14] = W[14];
            ROUND_00_15(14, c, d, e, f, g, h, a, b);
            T1 = X[15] = W[15];
            ROUND_00_15(15, b, c, d, e, f, g, h, a);

            data += ISSHE_SHA256_CBLOCK;
        } else {
            uint32_t l;

            (void)HOST_c2l(data, l);
            T1 = X[0] = l;
            ROUND_00_15(0, a, b, c, d, e, f, g, h);
            (void)HOST_c2l(data, l);
            T1 = X[1] = l;
            ROUND_00_15(1, h, a, b, c, d, e, f, g);
            (void)HOST_c2l(data, l);
            T1 = X[2] = l;
            ROUND_00_15(2, g, h, a, b, c, d, e, f);
            (void)HOST_c2l(data, l);
            T1 = X[3] = l;
            ROUND_00_15(3, f, g, h, a, b, c, d, e);
            (void)HOST_c2l(data, l);
            T1 = X[4] = l;
            ROUND_00_15(4, e, f, g, h, a, b, c, d);
            (void)HOST_c2l(data, l);
            T1 = X[5] = l;
            ROUND_00_15(5, d, e, f, g, h, a, b, c);
            (void)HOST_c2l(data, l);
            T1 = X[6] = l;
            ROUND_00_15(6, c, d, e, f, g, h, a, b);
            (void)HOST_c2l(data, l);
            T1 = X[7] = l;
            ROUND_00_15(7, b, c, d, e, f, g, h, a);
            (void)HOST_c2l(data, l);
            T1 = X[8] = l;
            ROUND_00_15(8, a, b, c, d, e, f, g, h);
            (void)HOST_c2l(data, l);
            T1 = X[9] = l;
            ROUND_00_15(9, h, a, b, c, d, e, f, g);
            (void)HOST_c2l(data, l);
            T1 = X[10] = l;
            ROUND_00_15(10, g, h, a, b, c, d, e, f);
            (void)HOST_c2l(data, l);
            T1 = X[11] = l;
            ROUND_00_15(11, f, g, h, a, b, c, d, e);
            (void)HOST_c2l(data, l);
            T1 = X[12] = l;
            ROUND_00_15(12, e, f, g, h, a, b, c, d);
            (void)HOST_c2l(data, l);
            T1 = X[13] = l;
            ROUND_00_15(13, d, e, f, g, h, a, b, c);
            (void)HOST_c2l(data, l);
            T1 = X[14] = l;
            ROUND_00_15(14, c, d, e, f, g, h, a, b);
            (void)HOST_c2l(data, l);
            T1 = X[15] = l;
            ROUND_00_15(15, b, c, d, e, f, g, h, a);
        }

        for (i = 16; i < 64; i += 8) {
            ROUND_16_63(i + 0, a, b, c, d, e, f, g, h, X);
            ROUND_16_63(i + 1, h, a, b, c, d, e, f, g, X);
            ROUND_16_63(i + 2, g, h, a, b, c, d, e, f, X);
            ROUND_16_63(i + 3, f, g, h, a, b, c, d, e, X);
            ROUND_16_63(i + 4, e, f, g, h, a, b, c, d, X);
            ROUND_16_63(i + 5, d, e, f, g, h, a, b, c, X);
            ROUND_16_63(i + 6, c, d, e, f, g, h, a, b, X);
            ROUND_16_63(i + 7, b, c, d, e, f, g, h, a, X);
        }

        state->state[0] += a;
        state->state[1] += b;
        state->state[2] += c;
        state->state[3] += d;
        state->state[4] += e;
        state->state[5] += f;
        state->state[6] += g;
        state->state[7] += h;

    }
}

int isshe_sha256_init(isshe_sha256_state_s *state)
{
    memset(state, 0, sizeof(*state));
    state->state[0] = 0x6a09e667UL;
    state->state[1] = 0xbb67ae85UL;
    state->state[2] = 0x3c6ef372UL;
    state->state[3] = 0xa54ff53aUL;
    state->state[4] = 0x510e527fUL;
    state->state[5] = 0x9b05688cUL;
    state->state[6] = 0x1f83d9abUL;
    state->state[7] = 0x5be0cd19UL;
    return 0;
}

int isshe_sha256_update(isshe_sha256_state_s *state, const uint8_t *data, uint64_t len)
{
    //const unsigned char *data = data_;
    unsigned char *p;
    uint32_t l;
    size_t n;

    if (len == 0)
        return 0;

    l = (state->Nl + (((uint32_t) len) << 3)) & 0xffffffffUL;
    if (l < state->Nl)              /* overflow */
        state->Nh++;
    
    // might cause compiler warning on 16-bit
    state->Nh += (uint32_t) (len >> 29); 
    state->Nl = l;

    n = state->num;
    if (n != 0) {
        p = (unsigned char *)state->data;

        if (len >= ISSHE_SHA256_CBLOCK || len + n >= ISSHE_SHA256_CBLOCK) {
            memcpy(p + n, data, ISSHE_SHA256_CBLOCK - n);
            sha256_block_data_order(state, p, 1);
            n = ISSHE_SHA256_CBLOCK - n;
            data += n;
            len -= n;
            state->num = 0;
            memset(p, 0, ISSHE_SHA256_CBLOCK); /* keep it zeroed */
        } else {
            memcpy(p + n, data, len);
            state->num += (unsigned int)len;
            return 0;
        }
    }

    n = len / ISSHE_SHA256_CBLOCK;
    if (n > 0) {
        sha256_block_data_order(state, data, n);
        n *= ISSHE_SHA256_CBLOCK;
        data += n;
        len -= n;
    }

    if (len != 0) {
        p = (unsigned char *)state->data;
        state->num = (unsigned int)len;
        memcpy(p, data, len);
    }
    return 0;
}

void isshe_sha256_transform(isshe_sha256_state_s *state, const unsigned char *data)
{
    sha256_block_data_order(state, data, 1);
}

int isshe_sha256_final(isshe_sha256_state_s *state, uint8_t result[32])
{
    unsigned char *p = (unsigned char *)state->data;
    size_t n = state->num;

    p[n] = 0x80;                /* there is always room for one */
    n++;

    if (n > (ISSHE_SHA256_CBLOCK - 8)) {
        memset(p + n, 0, ISSHE_SHA256_CBLOCK - n);
        n = 0;
        sha256_block_data_order(state, p, 1);
    }
    memset(p + n, 0, ISSHE_SHA256_CBLOCK - 8 - n);

    p += ISSHE_SHA256_CBLOCK - 8;
#if   defined(ISSHE_BIG_ENDIAN)
    (void)HOST_l2c(state->Nh, p);
    (void)HOST_l2c(state->Nl, p);
#elif defined(ISSHE_LITTLE_ENDIAN)
    (void)HOST_l2c(state->Nl, p);
    (void)HOST_l2c(state->Nh, p);
#endif
    p -= ISSHE_SHA256_CBLOCK;
    sha256_block_data_order(state, p, 1);
    state->num = 0;
    isshe_memzero(p, ISSHE_SHA256_CBLOCK);

    SHA256_MAKE_STRING(state, result);

    return 0;
}
int isshe_sha256(const uint8_t *data, uint64_t len, uint8_t result[32])
{
    isshe_sha256_state_s state;

    isshe_sha256_init(&state);
    isshe_sha256_update(&state, data, len);
    isshe_sha256_final(&state, result);
    isshe_memzero(&state, sizeof(state));
    return 0;
}