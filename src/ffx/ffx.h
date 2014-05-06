#ifndef _LIBFTE_SRC_FFX_FFX_H
#define _LIBFTE_SRC_FFX_FFX_H

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
  explicit FFX ( const uint32_t );

  const uint32_t get_radix() const {
    return radix_;
  }

  mpz_class encrypt ( const Key,
                      const mpz_class, const uint32_t );

  mpz_class decrypt ( const Key,
                      const mpz_class, const uint32_t );

  mpz_class encrypt ( const Key,
                      const mpz_class, const uint32_t,
                      const mpz_class, const uint32_t );

  mpz_class decrypt ( const Key,
                      const mpz_class, const uint32_t,
                      const mpz_class, const uint32_t );
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
