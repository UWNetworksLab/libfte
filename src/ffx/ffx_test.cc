#include "gtest/gtest.h"

#include "ffx/ffx.h"

const uint32_t kFfxRadix = 2;


TEST(FFX2, EncryptDecrypt1) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2, EncryptDecrypt2) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = mpz_class("19531846666280701709");
  uint32_t input_plaintext_len_in_bits = 65;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2, TestVector1) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_ui(), 97422040);
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector2) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = 1546594394;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_ui(), 1394942153);
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector3) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_ui(), 1475352427);
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector4) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
  mpz_class input_plaintext = 1546594394;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_ui(), 560889368);
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector5) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_ui(), 2017546936);
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector6) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class input_plaintext = 1546594394;
  mpz_class expected_ciphertext = mpz_class("3223057243");
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(expected_ciphertext.get_ui(), ciphertext.get_ui());
  EXPECT_EQ(input_plaintext.get_ui(), output_plaintext.get_ui());
}

TEST(FFX2, TestVector7) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = mpz_class("19531846666280701709");
  uint32_t input_plaintext_len_in_bits = 65;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(ciphertext.get_str(), mpz_class("24174057300333921796").get_str());
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2, TestVector8) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext =
    mpz_class("9999999999999999999999999999999999999999999999999999999999999");
  uint32_t input_plaintext_len_in_bits = 203;
  mpz_class expected_ciphertext = mpz_class("5482742996470928218251101700126162483085479022017991609519811");
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(expected_ciphertext.get_str(), ciphertext.get_str());
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2, TestVector9) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "0000000000000000FFFFFFFFFFFFFFFF";
  mpz_class input_plaintext =
    mpz_class("9999999999999999999999999999999999999999999999999999999999999");
  uint32_t input_plaintext_len_in_bits = 203;
  mpz_class expected_ciphertext = mpz_class("10938813410323079160848491505823845089434306691692900319087087");
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(expected_ciphertext.get_str(), ciphertext.get_str());
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2, TestBig1) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00000000000000000000000000000000";
  mpz_class input_plaintext = 0;
  uint32_t input_plaintext_len_in_bits = 1024 * 8;
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  ffxObj.Decrypt(key, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(FFX2Malicous, Shortkeyey1) {

  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "00";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext;
  bool success = ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  EXPECT_FALSE(success);
}

TEST(FFX2Malicous, Longkeyey1) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "0000000000000000000000000000000000000000";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext;
  bool success = ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  EXPECT_FALSE(success);
}

TEST(FFX2Malicous, Badkeyey1) {
  ffx::Ffx ffxObj = ffx::Ffx(kFfxRadix);
  std::string key = "0000000000000000000000000000000input_plaintext";
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext;
  bool success = ffxObj.Encrypt(key, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  EXPECT_FALSE(success);
}
