/*
 * See ffx.h for details about FFX. 
 */

#include <math.h>
#include <string.h>
#include <assert.h>

#include <gmpxx.h>

#include "aes/aes.h"

#include "ffx/ffx.h"

namespace ffx {

mpz_class F(const Key K,
            const uint32_t n,
            const mpz_class tweak,
            const uint32_t tweak_len,
            const uint32_t i,
            const mpz_class B,
            const uint32_t B_len) {

  mpz_class retval = 0;

  uint32_t vers = 1;
  uint32_t t = ceil(tweak_len / 8.0);
  uint32_t beta = ceil(n / 2.0);
  uint32_t b = ceil(beta / 8.0);
  uint32_t d = 4 * ceil(b / 4.0);

  uint32_t m = 0;
  if((i % 2) == 0) {
    m = floor(n / 2.0);
  } else {
    m = ceil(n / 2.0);
  }

  mpz_class P = 0;
  uint32_t P_len = (1 + 1 + 1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(vers)             << (1 + 1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(2)                << (1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(1)                << (3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(2)                << (1 + 1 + 4 + 4) * 8;
  P += mpz_class(10)               << (1 + 4 + 4) * 8;
  P += mpz_class(floor(n / 2.0))   << (4 + 4) * 8;
  P += mpz_class(n)                << (4) * 8;
  P += t;

  uint32_t B_bits = b * 8;

  mpz_class Q = 0;
  uint32_t T_offset = ((((-1 * t) - b - 1) % 16) * 8);
  T_offset += 8;
  T_offset += B_bits;

  uint32_t Q_len = tweak_len + T_offset;
  Q += mpz_class(tweak) << T_offset;
  Q += mpz_class(i) << B_bits;
  Q += B;

  mpz_class Y = 0;

  assert(((P_len / 8) % 16) == 0);
  assert(((Q_len / 8) % 16) == 0);

  Y = aes_cbc_mac(K, (P << Q_len) + Q, P_len + Q_len);

  mpz_class Z = Y;
  uint32_t Z_len = 16;
  mpz_class counter = 1;
  while(Z_len < (d + 4)) {
    mpz_class ctxt = aes_ecb_encrypt(K, (Y + counter), 128);
    Z_len += 16;
    Z = Z << 128;
    Z += ctxt;
    counter += 1;
  }

  Y = extract_bit_range(Z, Z_len * 8, 0, ((d + 4) * 8) - 1);

  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);
  Y = Y % modulus;

  retval = Y;

  return retval;
}

mpz_class FFX::encrypt(const Key key ,
                       const mpz_class tweak, const uint32_t tweak_len,
                       const mpz_class plaintext, const uint32_t plaintext_len) {

  if(key.length() != kFFXKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  mpz_class retval = 0;

  uint32_t n = plaintext_len;
  uint32_t l = floor(plaintext_len / 2.0);
  uint32_t r = DEFAULT_FFX_ROUNDS;
  mpz_class A = extract_bit_range(plaintext, plaintext_len, 0, l - 1);
  mpz_class B = extract_bit_range(plaintext, plaintext_len, l, n - 1);
  uint32_t B_len = n - l;
  uint32_t m = 0;
  mpz_class modulus = 0;
  mpz_class C = 0;
  uint32_t i = 0;
  for(i = 0; i <= (r - 1); i++) {
    if((i % 2) == 0) {
      m = floor(n / 2.0);
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    C = A + F(key, n, tweak, tweak_len, i, B, m);
    C = C % modulus;
    A = B;
    B = C;
  }

  retval = (A << B_len) + B;
  
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, plaintext_len);
  retval = retval % modulus;

  return retval;
}

mpz_class FFX::decrypt(const Key key,
                       const mpz_class tweak, const uint32_t tweak_len,
                       const mpz_class cihpertext, const uint32_t cihpertext_len) {

  if(key.length() != kFFXKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  mpz_class retval = 0;

  uint32_t n = cihpertext_len;
  uint32_t l = floor(cihpertext_len / 2.0);
  uint32_t r = DEFAULT_FFX_ROUNDS;
  mpz_class A = extract_bit_range(cihpertext, cihpertext_len, 0, l - 1);
  mpz_class B = extract_bit_range(cihpertext, cihpertext_len, l, n - 1);
  uint32_t B_len = n - l;
  uint32_t m = 0;
  mpz_class modulus = 0;
  mpz_class C = 0;
  int32_t i = 0;
  for(i = r - 1; i >= 0; i--) {
    if((i % 2) == 0) {
      m = floor(n / 2.0);
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    C = B;
    B = A;
    A = C - F(key, n, tweak, tweak_len, i, B, m);
    while(A < 0) A += modulus;
    A = A % modulus;
  }

  retval = (A << B_len) + B;
  
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, cihpertext_len);
  retval = retval % modulus;

  return retval;
}

mpz_class FFX::encrypt(const Key key,
                       const mpz_class plaintext,
                       const uint32_t plaintext_len) {
  return FFX::encrypt(key, 0, 0, plaintext, plaintext_len);
}

mpz_class FFX::decrypt(const Key key,
                       const mpz_class ciphertext,
                       const uint32_t ciphertext_len) {
  return FFX::decrypt(key, 0, 0, ciphertext, ciphertext_len);
}


} // namespace ffx
