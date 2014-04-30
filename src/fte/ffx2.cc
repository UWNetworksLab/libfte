#include <gmpxx.h>

#include "ffx2.h"

namespace fte {

mpz_class ffx2::encrypt( const std::string K, const mpz_class T, const mpz_class P ) {
    return P;
}

mpz_class ffx2::decrypt( const std::string K, const mpz_class T, const mpz_class C ) {
    return C;
}

mpz_class ffx2::encrypt( const std::string K, const mpz_class X ) {
    return ffx2::encrypt( K, 0, X );
}

mpz_class ffx2::decrypt( const std::string K, const mpz_class X ) {
    return ffx2::decrypt( K, 0, X );
}

}