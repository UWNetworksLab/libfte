#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "gtest/gtest.h"

#include "ffx/aes_ecb.h"

// Test vector from http://csrc.nist.gov/archive/aes/rijndael/rijndael-vals.zip

TEST(AES_ECB, Encrypt1) {
  ffx::Key key = "00000000000000000000000000000000";
  mpz_class plaintext = mpz_class("00000000000000000000000000000000",16);
  uint32_t plaintext_len = 128;
  mpz_class ciphertext = aes_ecb_encrypt(key, plaintext, plaintext_len);
  EXPECT_EQ(ciphertext.get_str(16), "66e94bd4ef8a2c3b884cfa59ca342b2e");
}

TEST(AES_ECB, Encrypt2) {
  ffx::Key key = "000102030405060708090A0B0C0D0E0F";
  mpz_class plaintext = mpz_class("000102030405060708090A0B0C0D0E0F",16);
  uint32_t plaintext_len = 128;
  mpz_class ciphertext = aes_ecb_encrypt(key, plaintext, plaintext_len);
  EXPECT_EQ(ciphertext.get_str(16), "a940bb5416ef045f1c39458c653ea5a");
}