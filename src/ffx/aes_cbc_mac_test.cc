#include "gtest/gtest.h"

#include "ffx/aes_cbc_mac.h"

// Test vector from http://csrc.nist.gov/archive/aes/rijndael/rijndael-vals.zip

TEST(AES_CBC_MAC, MAC1) {
  std::string key = "00000000000000000000000000000000";
  uint8_t base = 16;
  mpz_class plaintext = mpz_class("00000000000000000000000000000000", base);
  uint32_t plaintext_len = 128;
  mpz_class ciphertext = 0;
  ffx::AesCbcMac(key, plaintext, plaintext_len, &ciphertext);
  EXPECT_EQ(ciphertext.get_str(base), "66e94bd4ef8a2c3b884cfa59ca342b2e");
}
