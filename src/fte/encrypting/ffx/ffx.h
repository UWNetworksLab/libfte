/*
 * FFX is a Format-Preserving block cipher. It can be used to implement a
 * variable-input length blockcipher over an arbitrary radix.
 *
 * This is an implementation for FFX[radix=2] based on the proposed FFX NIST
 * standard, and the updated FFX specification by Rogaway et al. [2,3].
 *
 * Unfortunately, test vectors for FFX[2] do not exist. In this directory I've
 * included aes-ffx-vectors.txt, which has test vectors for radix=10,36. I've
 * used these test vectors to implement an FFX implementation in Python [1].
 * In turn, I used the Python implementation to generate FFX[2] vectors.
 *
 * TODO: This implementation only supports radix=2.
 * TODO: This implementation does not support tweaks.
 *
 * - Kevin P. Dyer
 *
 * [1] https://github.com/kpdyer/libffx
 * [2] http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec2.pdf
 * [3] http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec.pdf
 */

#ifndef _FFX_FFX_H
#define _FFX_FFX_H

#include "../encrypter.h"

#include <gmpxx.h>

#include "conversions.h"
#include "aes_ecb.h"
#include "aes_cbc_mac.h"

namespace fte {

namespace encrypting {

const uint32_t kDefaultFfxRadix = 2;
const uint32_t kDefaultFfxRounds = 10;
const uint32_t kFfxIvLengthInBytes = 16;
const uint32_t kFfxKeyLengthInBytes = 16;
const uint32_t kFfxKeyLengthInNibbles = kFfxKeyLengthInBytes * 2;
const uint32_t kFfxKeyLengthInBits = kFfxKeyLengthInBytes * 8;

class Ffx : public fte::encrypting::Encrypter {
 protected:
  bool RoundFunction(uint32_t n,
                     const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     uint32_t i,
                     const mpz_class & B,
                     uint32_t B_len,
                     mpz_class * retval);

  uint32_t radix_;
  uint32_t rounds_;
  unsigned char * key_;

 public:
  Ffx();
  Ffx(const uint32_t radix);
  Ffx(const uint32_t radix, const uint32_t rounds);

  /*
   * Returns the radix for the current FFX instantiation.
   */
  const uint32_t radix() {
    return radix_;
  }

  bool SetKey(const std::string & key);

  /*
   * On input of plaintext, plaintext_len_in_bits, performs FFX.Encrypt[radix]
   * w.r.t. to the key.
   *
   * This encrypt function preserves the length of the input plaintext. That is,
   * the resultant ciphertext will be a bitstring of length plaintext_len.
   */
  bool Encrypt(const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext);

  bool Encrypt(const mpz_class & tweak,
               uint32_t tweak_len_in_bits,
               const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext);

  /*
   * Given a ciphertext output from FFX.Encrypt[radix], a ciphertext_len and
   * key, recovers the input plaintext.
   */
  bool Decrypt(const mpz_class & ciphertext,
               uint32_t ciphertext_len_in_bits,
               mpz_class * plaintext);

  bool Decrypt(const mpz_class & tweak,
               uint32_t tweak_len_in_bits,
               const mpz_class & ciphertext,
               uint32_t ciphertext_len_in_bits,
               mpz_class * plaintext);
};

} // namespace fte

} // namespace encrypting

#endif /* _FTE_ENCRYPTER_H */
