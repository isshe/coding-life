#ifndef _ISSHE_SHA2_H_
#define _ISSHE_SHA2_H_
#pragma once

#include <stdint.h>
#include <stdio.h>

#define ISSHE_SHA256_LBLOCK             16
// SHA-256 treats input data as a contiguous
// array of 32 bit wide big-endian values.
#define ISSHE_SHA256_CBLOCK             64
#define ISSHE_SHA256_DIGEST_LENGTH      32

typedef struct {
    uint32_t state[8];
    uint32_t Nl, Nh;
    uint32_t data[ISSHE_SHA256_LBLOCK];
    uint32_t num;
} isshe_sha256_state_s;

int isshe_sha256_init(isshe_sha256_state_s *state);
int isshe_sha256_update(isshe_sha256_state_s *state, const uint8_t *data, uint64_t len);
void isshe_sha256_transform(isshe_sha256_state_s *state, const uint8_t *data);
int isshe_sha256_final(isshe_sha256_state_s *state, uint8_t result[32]);
int isshe_sha256(const uint8_t *data, uint64_t len, uint8_t result[32]);

#endif

