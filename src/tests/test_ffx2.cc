#include <gmpxx.h>

#include "gtest/gtest.h"

#include "fte/ffx2.h"

TEST(FFX2, ExtractBits1) {
    // 0b1000
    mpz_class X = 8;
    
    // should return exactly 1000b
    mpz_class retval = fte::extract_bit_range( X, 0, 3 );
    
    EXPECT_EQ( retval, 8 );
}

TEST(FFX2, ExtractBits2) {
    // 0b10001000
    mpz_class X = 136;
    
    // should return exactly 1000b for left/right
    mpz_class retval_left = fte::extract_bit_range( X, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, 4, 7 );
    
    EXPECT_EQ( retval_left, 8 );
    EXPECT_EQ( retval_right, 8 );
}

TEST(FFX2, ExtractBits3) {
    // 0b111111111
    mpz_class X = 255;
    
    // should return exactly 1111b for left/right
    mpz_class retval = fte::extract_bit_range( X, 0, 7 );
    
    EXPECT_EQ( retval, 255 );
}

TEST(FFX2, ExtractBits4) {
    // 0b111111111
    mpz_class X = 255;
    
    // should return exactly 1111b for left/right
    mpz_class retval_left = fte::extract_bit_range( X, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, 4, 7 );
    
    EXPECT_EQ( retval_left, 15 );
    EXPECT_EQ( retval_right, 15 );
}

TEST(FFX2, MpzCharConversion1) {
    uint32_t n = 2;
    mpz_class X = 1024;
    unsigned char * Y = new unsigned char[n];
    mpz_class Z = 0;
    
    fte::mpz_to_char_array( X, n, Y );
    
    EXPECT_EQ( n, 2 );
    EXPECT_EQ( Y[1], '\00' );
    EXPECT_EQ( Y[0], '\04' );
    
    fte::char_array_to_mpz( Y, n, Z );
    
    EXPECT_EQ( X, Z );
    
    delete[] Y;
}

TEST(FFX2, MpzCharConversion2) {
    mpz_class X = mpz_class("99999999999999999999");
    uint32_t n = mpz_sizeinbase(X.get_mpz_t(), 256);
    unsigned char * Y = new unsigned char[n];
    mpz_class Z = 0;
    
    fte::mpz_to_char_array( X, n, Y );
    fte::char_array_to_mpz( Y, n, Z );
    
    EXPECT_EQ( X, Z );
    
    delete[] Y;
}

TEST(FFX2, EncryptDecrypt1) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    std::string K = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    uint32_t Y_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    mpz_class Z = ffxObj.decrypt(K, Y, Y_len);
    EXPECT_EQ( X, Z );
}

TEST(FFX2, TestVector1) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    std::string K = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 97422040 );
}

TEST(FFX2, TestVector2) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    std::string K = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    mpz_class X = 1546594394;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 1394942153 );
}