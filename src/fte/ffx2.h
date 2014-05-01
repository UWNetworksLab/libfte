#ifndef _LIBFTE_SRC_FTE_FFX_H
#define _LIBFTE_SRC_FTE_FFX_H

#include <gmpxx.h>

namespace fte {
class ffx2 {
  public:
    ffx2() {};
    mpz_class encrypt( const std::string, const mpz_class, const uint32_t );
    mpz_class decrypt( const std::string, const mpz_class, const uint32_t );
    mpz_class encrypt( const std::string, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
    mpz_class decrypt( const std::string, const mpz_class, const uint32_t, const mpz_class, const uint32_t );
};
}

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */