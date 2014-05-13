/*
 * FFX is a Format-Preserving block cipher. It can be used to implement a
 * variable-input length blockcipher over an arbitrary radix.
 * 
 * This is an implementation for FFX[radix=2] based on the proposed FFX NIST
 * standard, and the updated FFX specification by Rogaway et al:
 * 
 * - http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec2.pdf
 * - http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec.pdf
 * 
 * Unfortunately, test vectors for FFX[2] do not exist. In this directory I've
 * included aes-ffx-vectors.txt, which has test vectors for radix=10,36. I've
 * used these test vectors to implement an FFX implementation in Python. In turn
 * I used the Python implementation to generate FFX[2] vectors.
 * 
 * TODO: This implementation only supports radix=2.
 * TODO: This implementation does not support tweaks.
 */

#ifndef _LIBFTE_SRC_FFX_FFX_H
#define _LIBFTE_SRC_FFX_FFX_H

#include <exception>

#include <gmpxx.h>

#include "ffx/conversions.h"
#include "ffx/encryption.h"
#include "ffx/key.h"

namespace ffx {

const uint32_t DEFAULT_FFX_RADIX = 2;
const uint32_t DEFAULT_FFX_ROUNDS = 10;

class FFX {
 private:
  uint32_t radix_;

 public:
  FFX();
  explicit FFX(const uint32_t);

  const uint32_t get_radix() const {
    return radix_;
  }

  mpz_class encrypt(const Key,
                    const mpz_class, const uint32_t);

  mpz_class decrypt(const Key,
                    const mpz_class, const uint32_t);

  mpz_class encrypt(const Key,
                    const mpz_class, const uint32_t,
                    const mpz_class, const uint32_t);

  mpz_class decrypt(const Key,
                    const mpz_class, const uint32_t,
                    const mpz_class, const uint32_t);
};

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

}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
