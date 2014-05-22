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
 * In turn I used the Python implementation to generate FFX[2] vectors.
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

#ifndef _LIBFTE_SRC_FFX_FFX_H
#define _LIBFTE_SRC_FFX_FFX_H

#include <exception>

#include <gmp.h>

#include "ffx/conversions.h"
#include "ffx/aes_ecb.h"
#include "ffx/aes_cbc_mac.h"
#include "ffx/key.h"


namespace ffx {

const uint32_t DEFAULT_FFX_RADIX = 2;
const uint32_t DEFAULT_FFX_ROUNDS = 10;

class FFXException : public std::exception {
  virtual const char* what() const throw() {
    return "FteException";
  }
};

class InvalidKeyLength : public FFXException {
  virtual const char* what() const throw() {
    return "Invalid key length.";
  }
};

class InvalidRadix : public FFXException {
  virtual const char* what() const throw() {
    return "We currently only support radix=2.";
  }
};

class FFX {
 private:
  uint32_t radix_;

 public:
  FFX()
    : radix_(DEFAULT_FFX_RADIX) {
  }

  FFX(const uint32_t radix)
    : radix_(radix) {
    if(radix_ != DEFAULT_FFX_RADIX) {
      throw InvalidRadix();
    }
  }

  /*
   * Returns the radix for the current FFX instantiation.
   */
  const uint32_t get_radix() const {
    return radix_;
  }

  /*
   * On input of plaintext, plaintext_len, performs FFX.Encrypt[radix]
   * w.r.t. to the key.
   * 
   * This encrypt function preserves the length of the input plaintext. That is,
   * the resultant ciphertext will be a bitstring of length plaintext_len.
   */
  mpz_class encrypt(const Key key,
                    const mpz_class plaintext,
                    const uint32_t plaintext_len);
  
  // tweak can be specified, but will be ignored
  mpz_class encrypt(const Key key,
                    const mpz_class tweak,
                    const uint32_t tweak_len,
                    const mpz_class plaintext,
                    const uint32_t plaintext_len);


  /*
   * Given a ciphertext output from FFX.Encrypt[radix], a ciphertext_len and
   * key, recovers the input plaintext.
   */
  mpz_class decrypt(const Key key,
                    const mpz_class ciphertext,
                    const uint32_t ciphertext_len);
  
  // tweak can be specified, but will be ignored
  mpz_class decrypt(const Key key,
                    const mpz_class tweak,
                    const uint32_t tweak_len,
                    const mpz_class ciphertext,
                    const uint32_t ciphertext_len);
};

}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
