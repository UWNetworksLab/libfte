/*
 * See ffx.h for details.
 *
 * References:
 *   [FFX2] http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec2.pdf
 */

#include "ffx/ffx.h"

#include <math.h>

#include <string>

#include "third_party/aes/aes.h"

namespace ffx {

static bool ValidateKey(const std::string & key) {
  if (key.length() != kFfxKeyLengthInNibbles) {
    return false;
  }
  for (uint32_t i = 0; i < key.length(); ++i) {
    if (isxdigit(key[i])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

/*
 * This function has an unfortunate number of input parameters.
 * However, this is a direct implementation of the round-function algorithm
 * F_K(n,T,i,B) defined in [FFX2].
 */
bool Ffx::RoundFunction(uint32_t n,
                        const mpz_class & tweak,
                        uint32_t tweak_len_in_bits,
                        uint32_t i,
                        const mpz_class & B,
                        uint32_t B_len,
                        mpz_class * retval) {

  // [FFX2] pg 3., line 31
  uint32_t t = (tweak_len_in_bits + 7) >> 3;
  uint32_t beta = (n + 1) >> 1;
  uint32_t b = (beta + 7) >> 3;
  uint32_t d = 4 * ((b + 3) >> 2);


  // [FFX2] pg 3., line 32
  uint32_t m = 0;
  if ((i & 1) == 0) {
    m = n >> 1;
  } else {
    m = (n + 1) >> 1;
  }

  // [FFX2] pg 3., line 33
  mpz_class P = 0;
  uint32_t P_len = (1 + 1 + 1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(1)      << (1 + 1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(2)      << (1 + 3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(1)      << (3 + 1 + 1 + 4 + 4) * 8;
  P += mpz_class(2)      << (1 + 1 + 4 + 4) * 8;
  P += mpz_class(10)     << (1 + 4 + 4) * 8;
  P += mpz_class(n / 2)  << (4 + 4) * 8;
  P += mpz_class(n)      << (4) * 8;
  P += t;

  uint32_t B_bits = b * 8;

  // [FFX2] pg 3., line 34
  mpz_class Q = 0;
  uint32_t T_offset = ((((-1 * t) - b - 1) % 16) * 8);
  T_offset += 8;
  T_offset += B_bits;

  uint32_t Q_len = tweak_len_in_bits + T_offset;
  Q += mpz_class(tweak) << T_offset;
  Q += mpz_class(i) << B_bits;
  Q += B;

  mpz_class Y = 0;

  // [FFX2] pg 3., line 35
  bool cbc_success = AesCbcMac(key_, (P << Q_len) + Q, P_len + Q_len, &Y);
  if (!cbc_success) {
    return false;
  }

  // [FFX2] pg 3., line 36
  mpz_class Z = Y;
  uint32_t Z_len = 16;
  mpz_class counter = 1;
  mpz_class ctxt = 0;
  while (Z_len < (d + 4)) {
    ctxt = 0;
    AesEcbEncrypt(key_, (Y + counter), 128, &ctxt);
    Z_len += 16;
    Z <<= 128;
    Z += ctxt;
    ++counter;
  }

  // [FFX2] pg 3., line 37
  BitMask(Z, Z_len * 8, 0, ((d + 4) * 8) - 1, &Y);

  // [FFX2] pg 3., line 3=8
  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);
  Y = Y % modulus;

  // [FFX2] pg 3., line 39
  *retval = Y;

  return true;
}

bool Ffx::Encrypt(const mpz_class & tweak,
                  uint32_t tweak_len_in_bits,
                  const mpz_class & plaintext,
                  uint32_t plaintext_len_in_bits,
                  mpz_class * ciphertext) {

  mpz_class & retval = *ciphertext;

  // [FFX2] pg 3., line 14-15
  uint32_t n = plaintext_len_in_bits;
  uint32_t l = plaintext_len_in_bits / 2;
  uint32_t r = kDefaultFfxRounds;
  mpz_class A, B;
  BitMask(plaintext, plaintext_len_in_bits, 0, l - 1, &A);
  BitMask(plaintext, plaintext_len_in_bits, l, n - 1, &B);
  uint32_t B_len = n - l;
  mpz_class C = 0;
  mpz_class D = 0;
  uint32_t m = 0;
  mpz_class modulus = 0;

  // [FFX2] pg 3., line 16
  for (uint32_t i = 0; i <= (r - 1); ++i) {
    if ((i & 1) == 0) {
      m = n / 2;
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    RoundFunction(n, tweak, tweak_len_in_bits, i, B, m, &D);
    mpz_add(C.get_mpz_t(),
            A.get_mpz_t(),
            D.get_mpz_t());

    C = C % modulus;
    A = B;
    B = C;
  }

  // [FFX2] pg 3., line 19
  retval = (A << B_len) + B;

  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, plaintext_len_in_bits);
  retval = retval % modulus;

  return true;
}

bool Ffx::Decrypt(const mpz_class & tweak,
                  uint32_t tweak_len_in_bits,
                  const mpz_class & ciphertext,
                  uint32_t ciphertext_len_bits,
                  mpz_class * plaintext) {

  mpz_class & retval = *plaintext;

  // [FFX2] pg 3., line 24-25
  uint32_t n = ciphertext_len_bits;
  uint32_t l = ciphertext_len_bits / 2;
  uint32_t r = kDefaultFfxRounds;
  mpz_class A, B;
  BitMask(ciphertext, ciphertext_len_bits, 0, l - 1, &A);
  BitMask(ciphertext, ciphertext_len_bits, l, n - 1, &B);
  uint32_t B_len = n - l;
  mpz_class C = 0;
  mpz_class D = 0;
  uint32_t m = 0;
  mpz_class modulus = 0;

  // [FFX2] pg 3., line 26
  for (int32_t i = r - 1; i >= 0; --i) {
    if ((i & 1) == 0) {
      m = n / 2;
    } else {
      m = ceil(n / 2.0);
    }
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m);

    C = B;
    B = A;
    RoundFunction(n, tweak, tweak_len_in_bits, i, B, m, &D);
    mpz_sub(A.get_mpz_t(),
            C.get_mpz_t(),
            D.get_mpz_t());

    while(A < 0) A += modulus;
    A = A % modulus;
  }

  // [FFX2] pg 3., line 29
  retval = (A << B_len) + B;

  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, ciphertext_len_bits);
  retval = retval % modulus;

  return true;
}

/*
 * These are the main entry points with NULL tweaks.
 */
bool Ffx::Encrypt(const mpz_class & plaintext,
                  uint32_t plaintext_len_in_bits,
                  mpz_class * ciphertext) {
  return Ffx::Encrypt(0, 0, plaintext, plaintext_len_in_bits, ciphertext);
}

bool Ffx::Decrypt(const mpz_class & ciphertext,
                  uint32_t ciphertext_len_in_bits,
                  mpz_class * plaintext) {
  return Ffx::Decrypt(0, 0, ciphertext, ciphertext_len_in_bits, plaintext);
}

bool Ffx::SetKey(const std::string & key) {
  // [FFX2] pg 3., line 21
  if (!ValidateKey(key)) {
    return false;
  }

  uint32_t key_length_in_bytes = (key.length() + 1) / 2;
  key_ = new unsigned char[key_length_in_bytes];
  Base16ToBase256(key, key_length_in_bytes, key_);

  return true;
}

Ffx::Ffx()
  : radix_(kDefaultFfxRadix) {
}

Ffx::Ffx(const uint32_t radix)
  : radix_(radix) {

}

} // namespace ffx
