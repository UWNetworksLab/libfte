#include <gmpxx.h>

#include "ffx2.h"

namespace fte {

mpz_class extract_bit_range( const mpz_class X, const uint32_t start, const uint32_t end ) {
    mpz_class retval = X;
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, end);
    retval = retval << start;
    retval = retval % modulus;
    return retval;
}

mpz_class ffx2::encrypt( const std::string K,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class P, const uint32_t P_len ) {
    
    uint32_t n = P_len; // input length of message
    uint32_t l = P_len / 2; // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( P, 0, l-1 );
    mpz_class B = extract_bit_range( P, l, n-1 );
    
    return P;
}

mpz_class ffx2::decrypt( const std::string K,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class C, const uint32_t C_len ) {
    return C;
}

mpz_class ffx2::encrypt( const std::string K,
                         const mpz_class X, const uint32_t X_len ) {
    return ffx2::encrypt( K, 0, 0, X, X_len );
}

mpz_class ffx2::decrypt( const std::string K,
                         const mpz_class X, const uint32_t X_len ) {
    return ffx2::decrypt( K, 0, 0, X, X_len );
}


}