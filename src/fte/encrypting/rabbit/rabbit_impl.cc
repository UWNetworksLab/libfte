/**
 * Rabbit.cc code is ported from rabbit.c by Cyptico A/S. The original code is
 * free for any use since 10/6, 2008.
 */

#include "rabbit.h"

#include <string.h>

#define U32V(v) ((uint32_t)(v) & 0x0FFFFFFFF)

#define ROTL32(v, n) (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define U32TO32_LITTLE(v) (v)

#define U8TO32_LITTLE(p) U32TO32_LITTLE(((uint32_t*)(p))[0])

/* -------------------------------------------------------------------------- */

/* Square a 32-bit uint32_teger to obtain the 64-bit result and return */
/* the upper 32 bits XOR the lower 32 bits */
static uint32_t RABBIT_g_func(uint32_t x) {
  /* Temporary variables */
  uint32_t a, b, h, l;

  /* Construct high and low argument for squaring */
  a = x & 0xFFFF;
  b = x >> 16;

  /* Calculate high and low result of squaring */
  h = (((U32V(a * a) >> 17) + U32V(a * b)) >> 15) + b * b;
  l = x * x;

  /* Return high XOR low */
  return U32V(h ^ l);
}

/* -------------------------------------------------------------------------- */

/* Calculate the next internal state */
static void RABBIT_next_state(RABBIT_ctx *p_instance) {
  /* Temporary variables */
  uint32_t g[8], c_old[8], i;

  /* Save old counter values */
  for (i = 0; i < 8; i++) {
    c_old[i] = p_instance->c[i];
  }

  /* Calculate new counter values */
  p_instance->c[0] =
    U32V(p_instance->c[0] + 0x4D34D34D + p_instance->carry);
  p_instance->c[1] =
    U32V(p_instance->c[1] + 0xD34D34D3 + (p_instance->c[0] < c_old[0]));
  p_instance->c[2] =
    U32V(p_instance->c[2] + 0x34D34D34 + (p_instance->c[1] < c_old[1]));
  p_instance->c[3] =
    U32V(p_instance->c[3] + 0x4D34D34D + (p_instance->c[2] < c_old[2]));
  p_instance->c[4] =
    U32V(p_instance->c[4] + 0xD34D34D3 + (p_instance->c[3] < c_old[3]));
  p_instance->c[5] =
    U32V(p_instance->c[5] + 0x34D34D34 + (p_instance->c[4] < c_old[4]));
  p_instance->c[6] =
    U32V(p_instance->c[6] + 0x4D34D34D + (p_instance->c[5] < c_old[5]));
  p_instance->c[7] =
    U32V(p_instance->c[7] + 0xD34D34D3 + (p_instance->c[6] < c_old[6]));
  p_instance->carry = (p_instance->c[7] < c_old[7]);

  /* Calculate the g-values */
  for (i = 0; i < 8; i++) {
    g[i] = RABBIT_g_func(U32V(p_instance->x[i] + p_instance->c[i]));
  }

  /* Calculate new state values */
  p_instance->x[0] = U32V(g[0] + ROTL32(g[7], 16) + ROTL32(g[6], 16));
  p_instance->x[1] = U32V(g[1] + ROTL32(g[0], 8) + g[7]);
  p_instance->x[2] = U32V(g[2] + ROTL32(g[1], 16) + ROTL32(g[0], 16));
  p_instance->x[3] = U32V(g[3] + ROTL32(g[2], 8) + g[1]);
  p_instance->x[4] = U32V(g[4] + ROTL32(g[3], 16) + ROTL32(g[2], 16));
  p_instance->x[5] = U32V(g[5] + ROTL32(g[4], 8) + g[3]);
  p_instance->x[6] = U32V(g[6] + ROTL32(g[5], 16) + ROTL32(g[4], 16));
  p_instance->x[7] = U32V(g[7] + ROTL32(g[6], 8) + g[5]);
}

/* ------------------------------------------------------------------------- */


void RabbitCryptor::keysetup(const uint8_t *key, uint32_t keysize,
                             uint32_t ivsize) {
  /* Temporary variables */
  uint32_t k0, k1, k2, k3, i;
  uint32_t key_int[4];

  memcpy(key_int, key, keysize);

  /* Generate four subkeys */
  k0 = U8TO32_LITTLE(key_int[0]);
  k1 = U8TO32_LITTLE(key_int[1]);
  k2 = U8TO32_LITTLE(key_int[2]);
  k3 = U8TO32_LITTLE(key_int[3]);

  /* Generate initial state variables */
  _master_ctx.x[0] = k0;
  _master_ctx.x[2] = k1;
  _master_ctx.x[4] = k2;
  _master_ctx.x[6] = k3;
  _master_ctx.x[1] = U32V(k3 << 16) | (k2 >> 16);
  _master_ctx.x[3] = U32V(k0 << 16) | (k3 >> 16);
  _master_ctx.x[5] = U32V(k1 << 16) | (k0 >> 16);
  _master_ctx.x[7] = U32V(k2 << 16) | (k1 >> 16);

  /* Generate initial counter values */
  _master_ctx.c[0] = ROTL32(k2, 16);
  _master_ctx.c[2] = ROTL32(k3, 16);
  _master_ctx.c[4] = ROTL32(k0, 16);
  _master_ctx.c[6] = ROTL32(k1, 16);
  _master_ctx.c[1] = (k0 & 0xFFFF0000) | (k1 & 0xFFFF);
  _master_ctx.c[3] = (k1 & 0xFFFF0000) | (k2 & 0xFFFF);
  _master_ctx.c[5] = (k2 & 0xFFFF0000) | (k3 & 0xFFFF);
  _master_ctx.c[7] = (k3 & 0xFFFF0000) | (k0 & 0xFFFF);

  /* Clear carry bit */
  _master_ctx.carry = 0;

  /* Iterate the system four times */
  for (i = 0; i < 4; i++) {
    RABBIT_next_state(&(_master_ctx));
  }

  /* Modify the counters */
  for (i = 0; i < 8; i++) {
    _master_ctx.c[i] ^= _master_ctx.x[(i + 4) & 0x7];
  }

  /* Copy master instance to work instance */
  for (i = 0; i < 8; i++) {
    _work_ctx.x[i] = _master_ctx.x[i];
    _work_ctx.c[i] = _master_ctx.c[i];
  }
  _work_ctx.carry = _master_ctx.carry;
}

/* ------------------------------------------------------------------------- */

/* IV setup */
void RabbitCryptor::ivsetup(const uint8_t *iv) {
  /* Temporary variables */
  uint32_t i0, i1, i2, i3, i;

  uint32_t iv_int[2];
  memcpy(iv_int, iv, 8);
  /* Generate four subvectors */
  i0 = U8TO32_LITTLE(iv_int[0]);
  i2 = U8TO32_LITTLE(iv_int[1]);
  i1 = (i0 >> 16) | (i2 & 0xFFFF0000);
  i3 = (i2 << 16) | (i0 & 0x0000FFFF);

  /* Modify counter values */
  _work_ctx.c[0] = _master_ctx.c[0] ^ i0;
  _work_ctx.c[1] = _master_ctx.c[1] ^ i1;
  _work_ctx.c[2] = _master_ctx.c[2] ^ i2;
  _work_ctx.c[3] = _master_ctx.c[3] ^ i3;
  _work_ctx.c[4] = _master_ctx.c[4] ^ i0;
  _work_ctx.c[5] = _master_ctx.c[5] ^ i1;
  _work_ctx.c[6] = _master_ctx.c[6] ^ i2;
  _work_ctx.c[7] = _master_ctx.c[7] ^ i3;

  /* Copy state variables */
  for (i = 0; i < 8; i++) {
    _work_ctx.x[i] = _master_ctx.x[i];
  }
  _work_ctx.carry = _master_ctx.carry;

  /* Iterate the system four times */
  for (i = 0; i < 4; i++) {
    RABBIT_next_state(&(_work_ctx));
  }
}

/* ------------------------------------------------------------------------- */

/* Encrypt/decrypt a message of any size */
void RabbitCryptor::process_bytes(int action, const uint8_t *input,
                                  uint8_t *output, uint32_t msglen) {
  /* Temporary variables */
  uint32_t i;
  uint8_t buffer[16];

  /* Encrypt/decrypt all full blocks */
  while (msglen >= 16) {
    /* Iterate the system */
    RABBIT_next_state(&(_work_ctx));

    /* Encrypt/decrypt 16 bytes of data */
    *(uint32_t *)(output + 0) = *(uint32_t *)(input + 0) ^
                                U32TO32_LITTLE(_work_ctx.x[0] ^ (_work_ctx.x[5] >> 16) ^
                                    U32V(_work_ctx.x[3] << 16));
    *(uint32_t *)(output + 4) = *(uint32_t *)(input + 4) ^
                                U32TO32_LITTLE(_work_ctx.x[2] ^ (_work_ctx.x[7] >> 16) ^
                                    U32V(_work_ctx.x[5] << 16));
    *(uint32_t *)(output + 8) = *(uint32_t *)(input + 8) ^
                                U32TO32_LITTLE(_work_ctx.x[4] ^ (_work_ctx.x[1] >> 16) ^
                                    U32V(_work_ctx.x[7] << 16));
    *(uint32_t *)(output + 12) = *(uint32_t *)(input + 12) ^
                                 U32TO32_LITTLE(_work_ctx.x[6] ^ (_work_ctx.x[3] >> 16) ^
                                     U32V(_work_ctx.x[1] << 16));

    /* Increment pointers and decrement length */
    input += 16;
    output += 16;
    msglen -= 16;
  }

  /* Encrypt/decrypt remaining data */
  if (msglen) {
    /* Iterate the system */
    RABBIT_next_state(&(_work_ctx));

    /* Generate 16 bytes of pseudo-random data */
    *(uint32_t *)(buffer + 0) = U32TO32_LITTLE(
                                  _work_ctx.x[0] ^ (_work_ctx.x[5] >> 16) ^ U32V(_work_ctx.x[3] << 16));
    *(uint32_t *)(buffer + 4) = U32TO32_LITTLE(
                                  _work_ctx.x[2] ^ (_work_ctx.x[7] >> 16) ^ U32V(_work_ctx.x[5] << 16));
    *(uint32_t *)(buffer + 8) = U32TO32_LITTLE(
                                  _work_ctx.x[4] ^ (_work_ctx.x[1] >> 16) ^ U32V(_work_ctx.x[7] << 16));
    *(uint32_t *)(buffer + 12) = U32TO32_LITTLE(
                                   _work_ctx.x[6] ^ (_work_ctx.x[3] >> 16) ^ U32V(_work_ctx.x[1] << 16));

    /* Encrypt/decrypt the data */
    for (i = 0; i < msglen; i++) {
      output[i] = input[i] ^ buffer[i];
    }
  }
}

/* ------------------------------------------------------------------------- */

/* Generate keystream */
void RabbitCryptor::keystream_bytes(uint8_t *keystream, uint32_t length) {
  /* Temporary variables */
  uint32_t i;
  uint8_t buffer[16];

  /* Generate all full blocks */
  while (length >= 16) {
    /* Iterate the system */
    RABBIT_next_state(&(_work_ctx));

    /* Generate 16 bytes of pseudo-random data */
    *(uint32_t *)(keystream + 0) = U32TO32_LITTLE(
                                     _work_ctx.x[0] ^ (_work_ctx.x[5] >> 16) ^ U32V(_work_ctx.x[3] << 16));
    *(uint32_t *)(keystream + 4) = U32TO32_LITTLE(
                                     _work_ctx.x[2] ^ (_work_ctx.x[7] >> 16) ^ U32V(_work_ctx.x[5] << 16));
    *(uint32_t *)(keystream + 8) = U32TO32_LITTLE(
                                     _work_ctx.x[4] ^ (_work_ctx.x[1] >> 16) ^ U32V(_work_ctx.x[7] << 16));
    *(uint32_t *)(keystream + 12) = U32TO32_LITTLE(
                                      _work_ctx.x[6] ^ (_work_ctx.x[3] >> 16) ^ U32V(_work_ctx.x[1] << 16));

    /* Increment pointers and decrement length */
    keystream += 16;
    length -= 16;
  }

  /* Generate remaining pseudo-random data */
  if (length) {
    /* Iterate the system */
    RABBIT_next_state(&(_work_ctx));

    /* Generate 16 bytes of pseudo-random data */
    *(uint32_t *)(buffer + 0) = U32TO32_LITTLE(
                                  _work_ctx.x[0] ^ (_work_ctx.x[5] >> 16) ^ U32V(_work_ctx.x[3] << 16));
    *(uint32_t *)(buffer + 4) = U32TO32_LITTLE(
                                  _work_ctx.x[2] ^ (_work_ctx.x[7] >> 16) ^ U32V(_work_ctx.x[5] << 16));
    *(uint32_t *)(buffer + 8) = U32TO32_LITTLE(
                                  _work_ctx.x[4] ^ (_work_ctx.x[1] >> 16) ^ U32V(_work_ctx.x[7] << 16));
    *(uint32_t *)(buffer + 12) = U32TO32_LITTLE(
                                   _work_ctx.x[6] ^ (_work_ctx.x[3] >> 16) ^ U32V(_work_ctx.x[1] << 16));

    /* Copy remaining data */
    for (i = 0; i < length; i++) {
      keystream[i] = buffer[i];
    }
  }
}

/* ------------------------------------------------------------------------- */

/* Encrypt/decrypt a number of full blocks */
void RabbitCryptor::process_blocks(int action, const uint8_t *input,
                                   uint8_t *output, uint32_t blocks) {
  /* Temporary variables */
  uint32_t i;

  for (i = 0; i < blocks; i++) {
    /* Iterate the system */
    RABBIT_next_state(&(_work_ctx));

    /* Encrypt/decrypt 16 bytes of data */
    *(uint32_t *)(output + 0) = *(uint32_t *)(input + 0) ^ U32TO32_LITTLE(
                                  _work_ctx.x[0] ^ (_work_ctx.x[5] >> 16) ^ U32V(_work_ctx.x[3] << 16));
    *(uint32_t *)(output + 4) = *(uint32_t *)(input + 4) ^ U32TO32_LITTLE(
                                  _work_ctx.x[2] ^ (_work_ctx.x[7] >> 16) ^ U32V(_work_ctx.x[5] << 16));
    *(uint32_t *)(output + 8) = *(uint32_t *)(input + 8) ^ U32TO32_LITTLE(
                                  _work_ctx.x[4] ^ (_work_ctx.x[1] >> 16) ^ U32V(_work_ctx.x[7] << 16));
    *(uint32_t *)(output + 12) = *(uint32_t *)(input + 12) ^ U32TO32_LITTLE(
                                   _work_ctx.x[6] ^ (_work_ctx.x[3] >> 16) ^ U32V(_work_ctx.x[1] << 16));

    /* Increment pointers to input and output data */
    input += 16;
    output += 16;
  }
}

/* ------------------------------------------------------------------------- */
