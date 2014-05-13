#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "gtest/gtest.h"

#include "ffx/ffx.h"

const uint32_t FFX_RADIX = 2;


TEST(FFX2, EncryptDecrypt1) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X = 1191613746;
  uint32_t X_len = 32;
  uint32_t Y_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  mpz_class Z = ffxObj.decrypt(K, Y, Y_len);
  EXPECT_EQ(X, Z);
}

TEST(FFX2, EncryptDecrypt2) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X = mpz_class("19531846666280701709");
  uint32_t X_len = 65;
  uint32_t Y_len = 65;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  mpz_class Z = ffxObj.decrypt(K, Y, Y_len);
  EXPECT_EQ(X, Z);
}

TEST(FFX2, TestVector1) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X = 1191613746;
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, 97422040);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector2) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X = 1546594394;
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, 1394942153);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector3) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
  mpz_class X = 1191613746;
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, 1475352427);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector4) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
  mpz_class X = 1546594394;
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, 560889368);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector5) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class X = 1191613746;
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, 2017546936);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector6) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class X = 1546594394;
  mpz_class ciphertext = mpz_class("3223057243");
  uint32_t X_len = 32;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, ciphertext);
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector7) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X = mpz_class("19531846666280701709");
  uint32_t X_len = 65;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y, mpz_class("24174057300333921796"));
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector8) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "00000000000000000000000000000000";
  mpz_class X =
    mpz_class("9999999999999999999999999999999999999999999999999999999999999");
  uint32_t X_len = 203;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y,
            mpz_class("5482742996470928218251101700126162483085479022017991609519811"));
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2, TestVector9) {
  ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
  ffx::Key K = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class X =
    mpz_class("9999999999999999999999999999999999999999999999999999999999999");
  uint32_t X_len = 203;
  mpz_class Y = ffxObj.encrypt(K, X, X_len);
  EXPECT_EQ(Y,
            mpz_class("10938813410323079160848491505823845089434306691692900319087087"));
  EXPECT_EQ(X, ffxObj.decrypt(K, Y, X_len));
}

TEST(FFX2Malicous, ShortKey1) {
  try {
    ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
    ffx::Key K = "00";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_TRUE(false);
  } catch(ffx::InvalidKeyLength e) {

  }
}

TEST(FFX2Malicous, LongKey1) {
  try {
    ffx::FFX ffxObj = ffx::FFX(FFX_RADIX);
    ffx::Key K = "0000000000000000000000000000000000000000";
    mpz_class X = 1191613746;
    uint32_t X_len = 32;
    mpz_class Y = ffxObj.encrypt(K, X, X_len);
    EXPECT_TRUE(false);
  } catch(ffx::InvalidKeyLength e) {

  }
}
