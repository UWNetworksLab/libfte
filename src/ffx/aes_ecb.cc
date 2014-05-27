#include "ffx/aes_ecb.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

bool AesEcbEncrypt(const std::string & key,
                   const mpz_class & plaintext,
                   uint32_t plaintext_len_in_bits,
                   mpz_class * ciphertext) {

  uint32_t plaintext_len_in_bytes = (plaintext_len_in_bits + 7) / 8;
  uint32_t key_length_in_bytes = (key.length() + 1) / 2;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pKey = new unsigned char[key_length_in_bytes];
  unsigned char * pInBuffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * pOutBuffer = new unsigned char[plaintext_len_in_bytes];

  for (uint32_t i = 0; i < plaintext_len_in_bytes; ++i) {
    pInBuffer[i] = 0x00;
    pOutBuffer[i] = 0x00;
  }

  for (uint32_t i = 0; i < key_length_in_bytes; ++i) {
    pKey[i] = 0x00;
  }

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, pInBuffer);
  Base16ToBase256(key, kFfxKeyLengthInBytes, pKey);

  aes_encrypt_key128(pKey, pCtx);
  aes_ecb_encrypt(pInBuffer, pOutBuffer, plaintext_len_in_bytes, pCtx);

  Base256ToMpzClass(pOutBuffer, plaintext_len_in_bytes, ciphertext);

  // cleanup
  delete[] pCtx;
  delete[] pKey;
  delete[] pInBuffer;
  delete[] pOutBuffer;

  pCtx = NULL;
  pKey = NULL;
  pInBuffer = NULL;
  pOutBuffer = NULL;

  return true;
}

} // namespace ffx
