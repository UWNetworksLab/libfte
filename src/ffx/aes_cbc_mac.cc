#include "ffx/aes_cbc_mac.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

bool AesCbcMac(const std::string & key,
               const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext) {

  uint32_t plaintext_len_in_bytes = (plaintext_len_in_bits + 7) / 8;
  uint32_t key_length_in_bytes = (key.length() + 1) / 2;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pIv = new unsigned char[key_length_in_bytes];
  unsigned char * pKey = new unsigned char[key_length_in_bytes];
  unsigned char * pInBuffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * pOutBuffer = new unsigned char[plaintext_len_in_bytes];

  for (uint32_t i = 0; i < plaintext_len_in_bytes; ++i) {
    pInBuffer[i] = 0x00;
    pOutBuffer[i] = 0x00;
  }

  for (uint32_t i = 0; i < kFfxKeyLengthInBytes; ++i) {
    pIv[i] = 0x00;
    pKey[i] = 0x00;
  }

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, pInBuffer);
  Base16ToBase256(key, key_length_in_bytes, pKey);

  aes_encrypt_key128(pKey, pCtx);
  aes_cbc_encrypt(pInBuffer, pOutBuffer, plaintext_len_in_bytes, pIv, pCtx);

  Base256ToMpzClass(pOutBuffer, plaintext_len_in_bytes, ciphertext);

  BitMask((*ciphertext),
          plaintext_len_in_bytes * 8,
          plaintext_len_in_bytes * 8 - 128,
          plaintext_len_in_bytes * 8 - 1,
          ciphertext);

  // cleanup
  delete[] pCtx;
  delete[] pIv;
  delete[] pKey;
  delete[] pInBuffer;
  delete[] pOutBuffer;

  pCtx = NULL;
  pIv = NULL;
  pKey = NULL;
  pInBuffer = NULL;
  pOutBuffer = NULL;

  return true;
}

} // namespace ffx
