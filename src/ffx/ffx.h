#ifndef _LIBFTE_SRC_FTE_FFX_H
#define _LIBFTE_SRC_FTE_FFX_H

#include <gmpxx.h>

#include "ffx/key.h"

namespace ffx {
    
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

mpz_class extract_bit_range( const mpz_class, const uint32_t, const uint32_t, const uint32_t );
void mpz_to_char_array( const mpz_class, const uint32_t, unsigned char * &);
void char_array_to_mpz( unsigned char *, const uint32_t, mpz_class & );

class ffx {
  private:
    uint32_t _radix;
  public:
    ffx();
    ffx(const uint32_t);
    const uint32_t getRadix() const { return _radix; }
    mpz_class encrypt( const key, const mpz_class, const uint32_t );
    mpz_class decrypt( const key, const mpz_class, const uint32_t );
    mpz_class encrypt( const key, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
    mpz_class decrypt( const key, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
};
}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
