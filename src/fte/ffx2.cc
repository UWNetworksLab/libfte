//#include <algorithm>
#include <math.h>
#include <string.h>
#include <iostream>

#include <gmpxx.h>

#include "ffx2.h"
#include "aes/aes.h"

namespace fte {

mpz_class extract_bit_range( const mpz_class X,
                             const uint32_t start, const uint32_t end ) {
    mpz_class retval = X;
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end+1) );
    retval = retval >> start;
    retval = retval % modulus;
    return retval;
}

//AES_RETURN aes_cbc_encrypt(const unsigned char *ibuf, unsigned char *obuf,
//                    int len, unsigned char *iv, const aes_encrypt_ctx cx[1]);

mpz_class CBC_MAC( const std::string K, const mpz_class X, const uint32_t X_len ) {
    mpz_class retval = 10;
    
    // X_len is 8x too much
    
    uint8_t i = 0;
    aes_encrypt_ctx ctx[1];
    unsigned char iv[16];
    char sKey[K.size()];
    char sInBuffer[X_len];
    size_t line_as_bytes_len = 0;
    //void* line_as_bytes =
        mpz_export(&sInBuffer, &line_as_bytes_len, 1, 1, 1, 0, X.get_mpz_t());
    
    unsigned char outBuffer[X_len];
   
    //strcpy( sInBuffer, plaintext.c_str() );
    strcpy( sKey, K.c_str() );
    
    const unsigned char * inBuffer = reinterpret_cast<const unsigned char *> (sInBuffer);
    const unsigned char * key = reinterpret_cast<const unsigned char *> (sKey);
    
    for(i = 0; i < 16; ++i)
        iv[i] = 0x00;
    
    aes_encrypt_key256(key, ctx);
    aes_cbc_encrypt(inBuffer, outBuffer, i, iv, ctx);
    
    //std::string ciphertext = std::string(reinterpret_cast<const char*>(outBuffer));

    //retval = mpz_class(ciphertext, 256);
        mpz_import(retval.get_mpz_t(), line_as_bytes_len, 1, 1, 1, 0, &outBuffer);
    
    return retval;
}

mpz_class F(const std::string K,
            const uint32_t n,
            const mpz_class T,
            const uint32_t T_len,
            const uint32_t i,
            const mpz_class B,
            const uint32_t B_len) {
    
    mpz_class retval = 0;
    
    uint32_t vers = 1;
    uint32_t t = T_len;
    uint32_t beta = ceil( n / 2.0 );
    uint32_t b = ceil( beta / 8.0 );
    uint32_t d = 4 * ceil( b / 4.0 );
    
    uint32_t m = 0;
    if ((i%2) == 0) {
        m = floor( m / 2.0 );
    } else {
        m = ceil( m / 2.0 );
    }
    
    mpz_class P = 0;
    uint32_t P_len = (1+1+1+3+1+1+4+4)*8;
    P += mpz_class(vers)             << (1+1+3+1+1+4+4)*8;
    P += mpz_class(2)                << (  1+3+1+1+4+4)*8;
    P += mpz_class(1)                << (    3+1+1+4+4)*8;
    P += mpz_class(2)                << (      1+1+4+4)*8;
    P += mpz_class(10)               << (        1+4+4)*8;
    P += mpz_class(floor( n / 2.0 )) << (          4+4)*8;
    P += mpz_class(n)                << (            4)*8;
    P += t;
    
    mpz_class Q = 0;
    uint32_t Q_len = (8 + (((-1*t - b - 1)%16)+1+B_len))*8;
    Q += T << ((((-1*t - b - 1)%16)+1+B_len)*8);
    Q += i << B_len;
    Q += B;
    
    mpz_class Y = 0;
    Y = CBC_MAC(K, (P<<Q_len) + Q, P_len + Q_len);
    //Y = first d+4 bytes of Y || AES_K(Y xor [1]^16) ...
    
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m );
    //Y = Y % modulus;
    
    retval = Y;
    
    return retval;
}


mpz_class ffx2::encrypt( const std::string K ,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class X, const uint32_t X_len ) {
    
    mpz_class retval = 0;
    
    uint32_t n = X_len; // input length of message
    uint32_t l = floor( X_len / 2.0); // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( X, 0, l-1 );
    mpz_class B = extract_bit_range( X, l, n-1 );
    uint32_t A_len = l;
    uint32_t B_len = (n-1) - l;
    
    mpz_class modulus = 0;
    //uint32_t block_size = std::max(A_len,B_len);
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, A_len ); 
    
    mpz_class C;
    uint32_t i = 0;
    for (i=0;i<=(r-1);i++) {
        std::cout << F(K,n,T,T_len,i,B,B_len) << std::endl;
        C = A + F(K,n,T,T_len,i,B,B_len);
        C = C % modulus;
        A = B;
        B = C;
    }
    
    retval = (A << B_len) + B;
    
    return retval;
}

mpz_class ffx2::decrypt( const std::string K,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class Y, const uint32_t Y_len ) {
    mpz_class retval = 0;
    
    uint32_t n = Y_len; // input length of message
    uint32_t l = floor( Y_len / 2.0 ); // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( Y, 0, l-1 );
    mpz_class B = extract_bit_range( Y, l, n-1 );
    uint32_t A_len = l;
    uint32_t B_len = (n-1) - l;
    
    mpz_class modulus = 0;
    //uint32_t block_size = std::max(A_len,B_len);
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, A_len ); 
    
    mpz_class C = 0;
    int32_t i = 0;
    for (i=r-1;i>=0;i--) {
        C = B;
        B = A;
        A = C - F(K,n,T,T_len,i,B,B_len);
        A = A % modulus;
    }
    
    retval = (A << B_len) + B;
    
    return retval;
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