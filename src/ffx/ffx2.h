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

mpz_class extract_bit_range( const mpz_class, const uint32_t, const uint32_t, const uint32_t );
void mpz_to_char_array( const mpz_class, const uint32_t, unsigned char * &);
void char_array_to_mpz( unsigned char *, const uint32_t, mpz_class & );

class ffx2 {
  public:
    ffx2() {};
    mpz_class encrypt( const ffx::key, const mpz_class, const uint32_t );
    mpz_class decrypt( const ffx::key, const mpz_class, const uint32_t );
    mpz_class encrypt( const ffx::key, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
    mpz_class decrypt( const ffx::key, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
};
}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
