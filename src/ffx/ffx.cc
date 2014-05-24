/*
 * See ffx.h for details about FFX.
 */

#include "ffx/ffx.h"

#include <math.h>

#include <string>

#include "third_party/aes/aes.h"

namespace ffx {

static bool RoundFunction(const std::string & K,
            uint32_t n,
            const mpz_class & tweak,
            uint32_t tweak_len_in_bits,
            uint32_t i,
            const mpz_class & B,
            uint32_t B_len,
            mpz_class * retval) {

  uint32_t vers = 1;
  uint32_t t = ceil(tweak_len_in_bits / 8.0);
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

  uint32_t Q_len = tweak_len_in_bits + T_offset;
  Q += mpz_class(tweak) << T_offset;
  Q += mpz_class(i) << B_bits;
  Q += B;

  mpz_class Y = 0;

  bool cbc_success = AesCbcMac(K, (P << Q_len) + Q, P_len + Q_len, &Y);
  if (!cbc_success) {
    return false;
  }
  mpz_class Z = Y;
  uint32_t Z_len = 16;
  mpz_class counter = 1;
  while(Z_len < (d + 4)) {
    mpz_class ctxt;
    AesEcbEncrypt(K, (Y + counter), 128, &ctxt);
    Z_len += 16;
    Z = Z << 128;
    Z += ctxt;
    ++counter;
  }

  Y = BitMask(Z, Z_len * 8, 0, ((d + 4) * 8) - 1);

  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);
  Y = Y % modulus;

  *retval = Y;
}

bool Ffx::Encrypt(const std::string & key ,
                  const mpz_class & tweak,
                  uint32_t tweak_len_in_bits,
                  const mpz_class & plaintext,
                  uint32_t plaintext_len_in_bits,
                  mpz_class * ciphertext) {

  if(key.length() != kFfxKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  mpz_class & retval = *ciphertext;

  uint32_t n = plaintext_len_in_bits;
  uint32_t l = floor(plaintext_len_in_bits / 2.0);
  uint32_t r = kDefaultFfxRounds;
  mpz_class A = BitMask(plaintext, plaintext_len_in_bits, 0, l - 1);
  mpz_class B = BitMask(plaintext, plaintext_len_in_bits, l, n - 1);
  uint32_t B_len = n - l;
  mpz_class C = 0;
  mpz_class D = 0;
  uint32_t m = 0;
  mpz_class modulus = 0;
  uint32_t i = 0;
  for(i = 0; i <= (r - 1); ++i) {
    if((i % 2) == 0) {
      m = floor(n / 2.0);
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    RoundFunction(key, n, tweak, tweak_len_in_bits, i, B, m, &D);
    C = A + D;
    C = C % modulus;
    A = B;
    B = C;
  }

  retval = (A << B_len) + B;

  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, plaintext_len_in_bits);
  retval = retval % modulus;
}

bool Ffx::Decrypt(const std::string & key,
                       const mpz_class & tweak,
                       uint32_t tweak_len_in_bits,
                       const mpz_class & cihpertext,
                       uint32_t cihpertext_len_bits,
                       mpz_class * plaintext) {

  if(key.length() != kFfxKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  mpz_class & retval = *plaintext;

  uint32_t n = cihpertext_len_bits;
  uint32_t l = floor(cihpertext_len_bits / 2.0);
  uint32_t r = kDefaultFfxRounds;
  mpz_class A = BitMask(cihpertext, cihpertext_len_bits, 0, l - 1);
  mpz_class B = BitMask(cihpertext, cihpertext_len_bits, l, n - 1);
  uint32_t B_len = n - l;
  mpz_class C = 0;
  mpz_class D = 0;
  uint32_t m = 0;
  mpz_class modulus = 0;
  int32_t i = 0;
  for(i = r - 1; i >= 0; --i) {
    if((i % 2) == 0) {
      m = floor(n / 2.0);
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    C = B;
    B = A;
    RoundFunction(key, n, tweak, tweak_len_in_bits, i, B, m, &D);
    A = C - D;
    while(A < 0) A += modulus;
    A = A % modulus;
  }

  retval = (A << B_len) + B;

  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, cihpertext_len_bits);
  retval = retval % modulus;
}

bool Ffx::Encrypt(const std::string & key,
                  const mpz_class & plaintext,
                  uint32_t plaintext_len_in_bits,
                  mpz_class * ciphertext) {
  return Ffx::Encrypt(key, 0, 0, plaintext, plaintext_len_in_bits, ciphertext);
}

bool Ffx::Decrypt(const std::string & key,
                  const mpz_class & ciphertext,
                  uint32_t ciphertext_len_in_bits,
                  mpz_class * plaintext) {
  return Ffx::Decrypt(key, 0, 0, ciphertext, ciphertext_len_in_bits, plaintext);
}


} // namespace ffx
