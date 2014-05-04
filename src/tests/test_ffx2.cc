#include <gmpxx.h>

#include "gtest/gtest.h"

#include "fte/exceptions.h"
#include "fte/ffx2.h"

TEST(FFX2, ExtractBits1) {
    // 0b1000
    mpz_class X = 0x08;
    uint8_t X_len = 4;

    // should return exactly 1000b
    mpz_class retval = fte::extract_bit_range( X, X_len, 0, 3 );

    EXPECT_EQ( retval, 0x8 );
}

TEST(FFX2, ExtractBits2) {
    // 0b10001000
    mpz_class X = 0x88;
    uint8_t X_len = 8;

    // should return exactly 1000b for left/right
    mpz_class retval_left = fte::extract_bit_range( X, X_len, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, X_len, 4, 7 );

    EXPECT_EQ( retval_left, 0x8 );
    EXPECT_EQ( retval_right, 0x8 );
}

TEST(FFX2, ExtractBits3) {
    // 0b111111111
    mpz_class X = 0xFF;
    uint8_t X_len = 8;

    // should return exactly 1111b for left/right
    mpz_class retval = fte::extract_bit_range( X, X_len, 0, 7 );

    EXPECT_EQ( retval, 0xFF );
}

TEST(FFX2, ExtractBits4) {
    // 0b111111111
    mpz_class X = 0xFF;
    uint8_t X_len = 8;

    // should return exactly 1111b for left/right
    mpz_class retval_left = fte::extract_bit_range( X, X_len, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, X_len, 4, 7 );

    EXPECT_EQ( retval_left, 0xF );
    EXPECT_EQ( retval_right, 0xF );
}

TEST(FFX2, ExtractBits5) {
    // 0b11110000
    mpz_class X = 0xF0;
    uint8_t X_len = 8;

    mpz_class retval_left = fte::extract_bit_range( X, X_len, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, X_len, 4, 7 );

    EXPECT_EQ( retval_left, 0xF );
    EXPECT_EQ( retval_right, 0x0 );
}

TEST(FFX2, ExtractBits6) {
    // 0b11110000
    mpz_class X = 0xFFFF0000;
    uint8_t X_len = 32;

    mpz_class retval_left = fte::extract_bit_range( X, X_len, 0, 15 );
    mpz_class retval_right = fte::extract_bit_range( X, X_len, 16, 31 );

    EXPECT_EQ( retval_left, 0xFFFF );
    EXPECT_EQ( retval_right, 0x0000 );
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
    fte::key K = "00000000000000000000000000000000";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    uint32_t Y_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    mpz_class Z = ffxObj.decrypt(K, Y, Y_len);
    EXPECT_EQ( X, Z );
}

TEST(FFX2, EncryptDecrypt2) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "00000000000000000000000000000000";
    mpz_class X = mpz_class("19531846666280701709");
    uint32_t X_len = 65;
    uint32_t Y_len = 65;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    mpz_class Z = ffxObj.decrypt(K, Y, Y_len);
    EXPECT_EQ( X, Z );
}

TEST(FFX2, TestVector1) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "00000000000000000000000000000000";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 97422040 );
}

TEST(FFX2, TestVector2) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "00000000000000000000000000000000";
    mpz_class X = 1546594394;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 1394942153 );
}

TEST(FFX2, TestVector3) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 1475352427 );
}

TEST(FFX2, TestVector4) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    mpz_class X = 1546594394;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 560889368 );
}

TEST(FFX2, TestVector5) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "0000000000000000FFFFFFFFFFFFFFFF";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 2017546936 );
}

TEST(FFX2, TestVector6) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "0000000000000000FFFFFFFFFFFFFFFF";
    mpz_class X = 1546594394;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, 3223057243 );
}

TEST(FFX2, TestVector7) {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "00000000000000000000000000000000";
    mpz_class X = mpz_class("19531846666280701709");
    uint32_t X_len = 65;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_EQ( Y, mpz_class("24174057300333921796") );
}

TEST(FFX2Malicous, ShortKey1) {
    try {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "00";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
        EXPECT_TRUE(false);
    } catch (fte::InvalidKeyLength e) {
    
    }
}

TEST(FFX2Malicous, LongKey1) {
    try {
    fte::ffx2 ffxObj = fte::ffx2::ffx2();
    fte::key K = "0000000000000000000000000000000000000000";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
        EXPECT_TRUE(false);
    } catch (fte::InvalidKeyLength e) {
    
    }
}