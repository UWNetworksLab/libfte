#include <math.h>
#include <string.h>
#include <iostream>
#include <assert.h>

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

void mpz_to_char_array( const mpz_class in, const uint32_t out_len, unsigned char * & out ) {
    mpz_class tmp = in;
    uint32_t i = (out_len-1);
    while (tmp>0) {
        mpz_class lowest;
        lowest = tmp%256;
        uint8_t byteholder = mpz_get_ui(lowest.get_mpz_t());
        out[i] = byteholder;
        //std::cout << i << " " << (uint32_t)byteholder << std::endl;
        tmp = tmp >> 8;
        i--;
    }
}

void char_array_to_mpz( unsigned char * in, const uint32_t in_len, mpz_class & out ) {
    out = 0;
    
    int32_t i = 0;
    for (i=0;i<in_len;i++) {
        mpz_class nugget = ((uint32_t)in[i]);
        //std::cout << i << " " << ((uint32_t)in[i]) << std::endl;
        out += nugget << 8*(in_len-1-i);
    }
}

//AES_RETURN aes_cbc_encrypt(const unsigned char *ibuf, unsigned char *obuf,
//                    int len, unsigned char *iv, const aes_encrypt_ctx cx[1]);

mpz_class CBC_MAC( const std::string K, const mpz_class X, const uint32_t X_len ) {
    mpz_class retval = 0;
    
    uint32_t byte_string_len = X_len / 8;
    assert( ((X_len / 8)%16) == 0 );
    //byte_string_len = ceil(byte_string_len/16.0)*16;
    
    uint8_t i = 0;
    aes_encrypt_ctx ctx[1];
    unsigned char iv[16];
    unsigned char key[16];
    //char sKey[K.size()];
    //char * sInBuffer;
    size_t line_as_bytes_len = 0;
    
    unsigned char * inBuffer = new unsigned char[byte_string_len];
    unsigned char * outBuffer = new unsigned char[byte_string_len];
    
    //std::cout << " X " << X << std::endl;
    //std::cout << " X_len " << X_len << std::endl;
    //std::cout << " byte_string_len " << byte_string_len << std::endl;
    for (i=0;i<byte_string_len;i++) {
        inBuffer[i] = 0;
        outBuffer[i] = 0;
    }
   
    mpz_to_char_array( X, byte_string_len, inBuffer );
    //strcpy( sKey, K.c_str() );
    
    for(i = 0; i < 16; ++i)
        key[i] = 0x00;
    
    for(i = 0; i < 16; ++i)
        iv[i] = 0x00;
    
    //for (i=0; i<16; i++) {
    //    std::cout << "key[]" << (uint32_t)key[i] << std::endl;
    //    std::cout << "iv[]" << (uint32_t)key[i] << std::endl;
    //}
    
    aes_encrypt_key128(key, ctx);
    aes_cbc_encrypt(inBuffer, outBuffer, byte_string_len, iv, ctx);
    
    //for (i=0;i<byte_string_len;i++) {
    //    std::cout << " in " << (uint32_t)inBuffer[i] << " out " << (uint32_t)outBuffer[i] << std::endl;
    //}
    
    char_array_to_mpz( outBuffer, byte_string_len, retval );
    
    //std::cout << "retval: " << retval << std::endl;
    
    //delete[] inBuffer;
    //delete[] outBuffer;
    
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
    
    uint32_t B_bits = ceil( B_len / 8.0 ) * 8;
    
    mpz_class Q = 0;
    uint32_t T_offset = ((((-1 * t) - b - 1) % 16) * 8) + 8 + B_bits;
    uint32_t Q_len = T_len + T_offset;
    Q += T << T_offset;
    Q += i << B_bits;
    Q += B;
    
    mpz_class Y = 0;
    
    //std::cout << " i " << i << std::endl;
    //std::cout << " B " << B << std::endl;
    //std::cout << " P_len " << P_len << std::endl;
    //std::cout << " Q_len " << Q_len << std::endl;
    //std::cout << " P " << P << std::endl;
    //std::cout << " Q " << Q << std::endl;
    //std::cout << " T_offset " << T_offset << std::endl;
    //std::cout << " B_len " << B_len << std::endl;
    //std::cout << " T_len " << T_len << std::endl;
    //std::cout << " t " << t << std::endl;
    //std::cout << " 0 " << (((-1 * t) - b - 1) % 16) << std::endl;
    //std::cout << " i " << 1 << std::endl;
    //std::cout << " b " << b << std::endl;
    //std::cout << " d+4 " << d+4 << std::endl;
    
    
    Y = CBC_MAC(K, (P<<Q_len) + Q, P_len + Q_len);
    //Y = first d+4 bytes of Y || AES_K(Y xor [1]^16) ...
    
    
    mpz_class Z = Y;
    uint32_t Z_len = 16;
    while (Z_len<(d+4)) {
        assert(false);
        //Z = Z << 128;
        //ctxt = AES(K, Y^[1]^16)
        //Z += ctxt;
    }
    
    //Y = extract_bit_range( Z, Z_len*8 - (d+4)*8, (Z_len*8)-1 ) ;
    //Y = extract_bit_range( Z, 0, ((d+4)*8)-1 ) ;
    //Y = extract_bit_range( Z, 64, 127 ) ;
    Y = extract_bit_range( Z, 64, 63 ) ;
    //std::cout << " XXXXXXX " << Y << std::endl;
    
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, m );
    //std::cout << " modulus " << modulus << std::endl;
    //std::cout << " m " << m << std::endl;
    //std::cout << " Y_bef " << Y << std::endl;
    Y = Y % modulus;
    //std::cout << " Y_aft " << Y << std::endl;
    
    retval = Y;
    
    return retval;
}


mpz_class ffx2::encrypt( const std::string K ,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class X, const uint32_t X_len ) {
    
    mpz_class retval = 0;
    
    uint32_t n = X_len; // input length of message
    uint32_t l = floor( X_len / 2.0 ); // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( X, l, n-1 );
    mpz_class B = extract_bit_range( X, 0, l-1 );
    uint32_t A_len = l;
    uint32_t B_len = (n-1) - l;
    
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, A_len ); 
    
    mpz_class C;
    uint32_t i = 0;
    for (i=0;i<=(r-1);i++) {
        //std::cout << " F " << F(K,n,T,T_len,i,B,B_len) << std::endl;
        /*std::cout << " l " << l << std::endl;
        std::cout << " F " << F(K,n,T,T_len,i,B,B_len) << std::endl;
        std::cout << " n " << n << std::endl;
        std::cout << " T " << T << std::endl;
        std::cout << " i " << i << std::endl;*/
        //std::cout << " A " << A << std::endl;
        //std::cout << " B " << B << std::endl;
        C = A + F(K,n,T,T_len,i,B,B_len);
        C = C % modulus;
        A = B;
        B = C;
    }
    
        //std::cout << " A " << A << std::endl;
        //std::cout << " B " << B << std::endl;
    
    retval = (A << A_len) + B;
    
    return retval;
}

mpz_class ffx2::decrypt( const std::string K,
                         const mpz_class T, const uint32_t T_len,
                         const mpz_class Y, const uint32_t Y_len ) {
    mpz_class retval = 0;
    
    uint32_t n = Y_len; // input length of message
    uint32_t l = floor( Y_len / 2.0 ); // maximally-balanced feistel
    uint32_t r = 10; // rounds
    mpz_class A = extract_bit_range( Y, l, n-1 );
    mpz_class B = extract_bit_range( Y, 0, l-1 );
    uint32_t A_len = l;
    uint32_t B_len = (n-1) - l;
    
    mpz_class modulus = 0;
    mpz_ui_pow_ui(modulus.get_mpz_t(), 2, A_len ); 
    
    mpz_class C = 0;
    int32_t i = 0;
    for (i=r-1;i>=0;i--) {
        C = B;
        B = A;
        A = C - F(K,n,T,T_len,i,B,B_len);
        while (A<0) A += modulus;
        A = A % modulus;
    }
    
    retval = (A << A_len) + B;
    
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