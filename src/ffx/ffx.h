#ifndef _LIBFTE_SRC_FFX_FFX_H
#define _LIBFTE_SRC_FFX_FFX_H

#include <gmpxx.h>

#include "ffx/conversions.h"
#include "ffx/encryption.h"
#include "ffx/key.h"

namespace ffx {

const uint32_t DEFAULT_FFX_RADIX = 2;
const uint32_t DEFAULT_FFX_ROUNDS = 10;
const uint32_t FTE_KEY_LENGTH_IN_NIBBLES = 32;

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

class ffx {
  private:
    uint32_t _radix;
 
  public:
    ffx();
    ffx(const uint32_t);
    const uint32_t getRadix() const {
        return _radix;
    }
    
    mpz_class encrypt( const key,
                       const mpz_class, const uint32_t );
    
    mpz_class decrypt( const key,
                       const mpz_class, const uint32_t );
    
    mpz_class encrypt( const key,
                       const mpz_class, const uint32_t,
                       const mpz_class, const uint32_t );
    
    mpz_class decrypt( const key,
                       const mpz_class, const uint32_t,
                       const mpz_class, const uint32_t );
};
}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
