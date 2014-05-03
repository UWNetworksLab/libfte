#include <math.h>
#include <string.h>
#include <assert.h>
#include <iostream>

#include <gmpxx.h>

#include "ffx2.h"
#include "aes/aes.h"

namespace fte {

mpz_class extract_bit_range( const mpz_class X, const uint32_t X_len,
                             const uint32_t start, const uint32_t end ) {
    mpz_class retval = X;
    retval = retval >> (X_len-end-1);
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end-start+1) );
    retval = retval % modulus;
    return retval;
}

void mpz_to_char_array( const mpz_class in, const uint32_t out_len, unsigned char * & out ) {
    mpz_class tmp = in;
    uint32_t i = (out_len-1);
    while (tmp>0) {
        mpz_class lowest;
        lowest = tmp%256;
        uint8_t byteholder = mpz_get_ui(lowest.get_mpz_t());
        out[i] = byteholder;
        tmp = tmp >> 8;
        i--;
    }
}

void char_array_to_mpz( unsigned char * in, const uint32_t in_len, mpz_class & out ) {
    out = 0;
    int32_t i = 0;
    for (i=0; i<in_len; i++) {
        mpz_class nugget = ((uint32_t)in[i]);
        out += nugget << 8*(in_len-1-i);
    }
}

mpz_class CBC_MAC( const std::string K, const mpz_class X, const uint32_t X_len ) {
    mpz_class retval = 0;
    
    uint32_t byte_string_len = X_len / 8;
    assert( ((X_len / 8)%16) == 0 );

    uint8_t i = 0;
    aes_encrypt_ctx ctx[1];
    unsigned char iv[16];
    unsigned char key[16];
    size_t line_as_bytes_len = 0;

    unsigned char * inBuffer = new unsigned char[byte_string_len];
    unsigned char * outBuffer = new unsigned char[byte_string_len];

    for (i=0; i<byte_string_len; i++) {
        inBuffer[i] = 0;
        outBuffer[i] = 0;
    }

    mpz_to_char_array( X, byte_string_len, inBuffer );
    
    for(i = 0; i < 16; ++i) {
        key[i] = 0x00;
    }
    
    // probably not safe?
    std::copy(K.begin(), K.end(), key);
   
    for(i = 0; i < 16; ++i) {
        iv[i] = 0x00;
    }

    aes_init();
    aes_encrypt_key128(key, ctx);
    aes_cbc_encrypt(inBuffer, outBuffer, byte_string_len, iv, ctx);

    char_array_to_mpz( outBuffer, byte_string_len, retval );
    
    retval = extract_bit_range( retval, byte_string_len*8, byte_string_len*8 - 128, byte_string_len*8 - 1);

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
    uint32_t t = ceil( T_len / 8.0 );
    uint32_t beta = ceil( n / 2.0 );
    uint32_t b = ceil( beta / 8.0 );
    uint32_t d = 4 * ceil( b / 4.0 );

    uint32_t m = 0;
    if ((i%2) == 0) {
        m = floor( n / 2.0 );
    } else {
        m = ceil( n / 2.0 );
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

    uint32_t B_bits = b * 8;

    mpz_class Q = 0;
    uint32_t T_offset = ((((-1 * t) - b - 1) % 16) * 8);
    T_offset += 8;
    T_offset += B_bits;
    
    uint32_t Q_len = T_len + T_offset;
    Q += T << T_offset;
    Q += i << B_bits;
    Q += B;

    mpz_class Y = 0;

    /*std::cout << "b " << b << std::endl;
    std::cout << "i " << i << std::endl;
    std::cout << "m " << m << std::endl;
    std::cout << "n " << n << std::endl;
    std::cout << "t " << t << std::endl;
    std::cout << "B_len " << B_len << std::endl;
    std::cout << "B_bits " << B_bits << std::endl;
    std::cout << "P_len " << P_len << std::endl;
    std::cout << "Q_len " << Q_len << std::endl;
    std::cout << "T_len " << T_len << std::endl;
    std::cout << "T_offset " << T_offset << std::endl;*/
    
    assert( ((P_len / 8)%16) == 0 );
    assert( ((Q_len / 8)%16) == 0 );
    
    Y = CBC_MAC(K, (P<<Q_len) + Q, P_len + Q_len);

    //std::cout << "Y1 " << Y << std::endl;

    mpz_class Z = Y;
    uint32_t Z_len = 16;
    while (Z_len<(d+4)) {
        assert(false);
        //Z = Z << 128;
        //ctxt = AES(K, Y^[1]^16)
        //Z += ctxt;
    }
    
    Y = extract_bit_range( Z, Z_len*8, 0, (d+4)*8-1 ) ;

    //std::cout << "Y2 " << Y << std::endl;

    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m );
    Y = Y % modulus;

    //std::cout << "Y3 " << Y << std::endl;
    
    retval = Y;

    return retval;
}


mpz_class ffx2::encrypt( const std::string K ,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class X, const uint32_t X_len ) {

    assert(X>0);
    
    mpz_class retval = 0;

    uint32_t n = X_len; // input length of message
    uint32_t l = floor( X_len / 2.0 ); // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( X, X_len, 0, l-1 );
    mpz_class B = extract_bit_range( X, X_len, l, n-1 );
    uint32_t A_len = l;
    uint32_t B_len = n-l;

    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, std::max(A_len,B_len) );

    mpz_class C = 0;
    uint32_t i = 0;
    for (i=0; i<=(r-1); i++) {
        //std::cout << " F " << F(K,n,T,T_len,i,B,B_len) << std::endl;
        //std::cout << " A B " << A << " " << B << std::endl;
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
    mpz_class A = extract_bit_range( Y, Y_len, 0, l-1 );
    mpz_class B = extract_bit_range( Y, Y_len, l, n-1 );
    uint32_t A_len = l;
    uint32_t B_len = n-l;

    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, std::max(A_len,B_len) );

    mpz_class C = 0;
    int32_t i = 0;
    for (i=r-1; i>=0; i--) {
        C = B;
        B = A;
        A = C - F(K,n,T,T_len,i,B,B_len);
        while (A<0) A += modulus;
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