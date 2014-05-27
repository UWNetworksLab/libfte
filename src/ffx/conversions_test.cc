#include "gtest/gtest.h"

#include "ffx/ffx.h"

TEST(FFX2, ExtractBits1) {
  // X is the 4-bit string 0b1000
  mpz_class X = 0x8;
  uint8_t X_len_in_bits = 4;

  // should return exactly 1000b (bits in index 0 to 3)
  mpz_class retval;
  ffx::BitMask(X, X_len_in_bits, 0, 3, &retval);

  EXPECT_EQ(retval.get_ui(), 0x8);
}

TEST(FFX2, ExtractBits2) {
  // X is the 8-bit string: 0b10000111
  mpz_class X = 0x87;
  uint8_t X_len_in_bits = 8;

  // should return exactly 1000b for left/right
  mpz_class retval_left, retval_right;
  ffx::BitMask(X, X_len_in_bits, 0, 3, &retval_left);
  ffx::BitMask(X, X_len_in_bits, 4, 7, &retval_right);

  EXPECT_EQ(retval_left.get_ui(), 0x8);
  EXPECT_EQ(retval_right.get_ui(), 0x7);
}

TEST(FFX2, ExtractBits3) {
  // 0b00001111
  mpz_class X = 0x0F;
  uint8_t X_len_in_bits = 8;

  // should return exactly 00001111b
  mpz_class retval;
  ffx::BitMask(X, X_len_in_bits, 0, 7, &retval);

  EXPECT_EQ(retval.get_ui(), 0x0F);
}

TEST(FFX2, ExtractBits4) {
  // 0b11110000
  mpz_class X = 0xFFFF0000;
  uint8_t X_len_in_bits = 32;

  mpz_class retval_left, retval_right;
  ffx::BitMask(X, X_len_in_bits, 0, 15, &retval_left);
  ffx::BitMask(X, X_len_in_bits, 16, 31, &retval_right);

  EXPECT_EQ(retval_left.get_ui(), 0xFFFF);
  EXPECT_EQ(retval_right.get_ui(), 0x0000);
}

TEST(FFX2, MpzCharConversion1) {
  mpz_class X = 0x0400;
  uint32_t Y_len_in_bytes = 2;
  unsigned char * Y = new unsigned char[Y_len_in_bytes];
  mpz_class Z = 0;

  ffx::MpzClassToBase256(X, Y_len_in_bytes, Y);

  EXPECT_EQ(Y[1], 0x00);
  EXPECT_EQ(Y[0], 0x04);

  ffx::Base256ToMpzClass(Y, Y_len_in_bytes, &Z);

  EXPECT_EQ(X.get_str(), Z.get_str());

  delete[] Y;
}

TEST(FFX2, MpzCharConversion2) {
  mpz_class X = mpz_class("99999999999999999999");
  uint32_t base = 256;
  uint32_t Y_len_in_bytes = mpz_sizeinbase(X.get_mpz_t(), base);
  unsigned char * Y = new unsigned char[Y_len_in_bytes];
  mpz_class Z = 0;

  ffx::MpzClassToBase256(X, Y_len_in_bytes, Y);
  ffx::Base256ToMpzClass(Y, Y_len_in_bytes, &Z);

  EXPECT_EQ(X.get_str(), Z.get_str());

  delete[] Y;
}

TEST(FFX2, 16to256) {
  std::string X = "00000000000000000000000000000000";
  uint32_t sizeInBase256 = 16;
  unsigned char * Y = new unsigned char[sizeInBase256];

  ffx::Base16ToBase256(X, sizeInBase256, Y);

  for (uint32_t i = 0; i<sizeInBase256; ++i) {
    EXPECT_EQ(Y[i], 0);
  }

  delete[] Y;
}
