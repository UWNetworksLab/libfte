#include "gtest/gtest.h"

#include "rabbit.h"


TEST(RABBIT2, EncryptDecrypt1) {
  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "00000000000000000000000000000000";
  rabbitObj.SetKey(key);
  mpz_class input_plaintext = 1191613746;
  uint32_t input_plaintext_len_in_bits = 32;
  mpz_class ciphertext, output_plaintext;
  rabbitObj.Encrypt(0, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  rabbitObj.Decrypt(0, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(RABBIT2, EncryptDecrypt2) {
  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "00000000000000000000000000000000";
  rabbitObj.SetKey(key);
  mpz_class input_plaintext = mpz_class("19531846666280701709");
  uint32_t input_plaintext_len_in_bits = 65;
  mpz_class ciphertext, output_plaintext;
  rabbitObj.Encrypt(0, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  rabbitObj.Decrypt(0, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(RABBIT2, TestBig1) {
  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "00000000000000000000000000000000";
  rabbitObj.SetKey(key);
  mpz_class input_plaintext = 0;
  uint32_t input_plaintext_len_in_bits = 1024 * 8;
  mpz_class ciphertext, output_plaintext;
  rabbitObj.Encrypt(0, input_plaintext, input_plaintext_len_in_bits, &ciphertext);
  rabbitObj.Decrypt(0, ciphertext, input_plaintext_len_in_bits, &output_plaintext);
  EXPECT_EQ(input_plaintext.get_str(), output_plaintext.get_str());
}

TEST(RABBIT2Malicous, Shortkeyey1) {

  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "00";
  bool success = rabbitObj.SetKey(key);
  EXPECT_FALSE(success);
}

TEST(RABBIT2Malicous, Longkeyey1) {
  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "0000000000000000000000000000000000000000";
  bool success = rabbitObj.SetKey(key);
  EXPECT_FALSE(success);
}

TEST(RABBIT2Malicous, Badkeyey1) {
  fte::encrypting::Rabbit rabbitObj = fte::encrypting::Rabbit();
  std::string key = "0000000000000000000000000000000BADKEY";
  bool success = rabbitObj.SetKey(key);
  EXPECT_FALSE(success);
}
