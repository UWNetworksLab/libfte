#ifndef _LIBFTE_SRC_FTE_FFX_H
#define _LIBFTE_SRC_FTE_FFX_H

#include <gmpxx.h>

namespace fte {
class ffx2 {
  public:
    ffx2() {};
    mpz_class encrypt( const std::string, const mpz_class );
    mpz_class decrypt( const std::string, const mpz_class );
    mpz_class encrypt( const std::string, const mpz_class, const mpz_class );
    mpz_class decrypt( const std::string, const mpz_class, const mpz_class );
};
}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */