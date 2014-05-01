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
    
    // should return exactly 1000b for left/right
    mpz_class retval = fte::extract_bit_range( X, 0, 7 );
    
    EXPECT_EQ( retval, 255 );
}

TEST(FFX2, ExtractBits4) {
    // 0b111111111
    mpz_class X = 255;
    
    // should return exactly 1000b for left/right
    mpz_class retval_left = fte::extract_bit_range( X, 0, 3 );
    mpz_class retval_right = fte::extract_bit_range( X, 4, 7 );
    
    EXPECT_EQ( retval_left, 15 );
    EXPECT_EQ( retval_right, 15 );
}