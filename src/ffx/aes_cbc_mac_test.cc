#include "gtest/gtest.h"

#include "ffx/aes_cbc_mac.h"

// Test vector from http://csrc.nist.gov/archive/aes/rijndael/rijndael-vals.zip

TEST(AES_CBC_MAC, MAC1) {
  std::string key = "00000000000000000000000000000000";
  mpz_class plaintext = mpz_class("00000000000000000000000000000000",16);
  uint32_t plaintext_len = 128;
  mpz_class ciphertext = ffx::AesCbcMac(key, plaintext, plaintext_len);
  EXPECT_EQ(ciphertext.get_str(16), "66e94bd4ef8a2c3b884cfa59ca342b2e");
}
