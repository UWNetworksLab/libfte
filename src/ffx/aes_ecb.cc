#include "ffx/aes_ecb.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

bool AesEcbEncrypt(unsigned char * key,
                   const mpz_class & plaintext,
                   uint32_t plaintext_len_in_bits,
                   mpz_class * ciphertext) {

  uint32_t plaintext_len_in_bytes = (plaintext_len_in_bits + 7) / 8;
  uint32_t ciphertext_len_in_bytes = plaintext_len_in_bytes;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pInBuffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * pOutBuffer = new unsigned char[ciphertext_len_in_bytes];

  for (uint32_t i = 0; i < plaintext_len_in_bytes; ++i) {
    pInBuffer[i] = 0x00;
    pOutBuffer[i] = 0x00;
  }

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, pInBuffer);

  aes_init();
  aes_encrypt_key128(key, pCtx);
  aes_ecb_encrypt(pInBuffer, pOutBuffer, plaintext_len_in_bytes, pCtx);

  Base256ToMpzClass(pOutBuffer, ciphertext_len_in_bytes, ciphertext);

  // cleanup
  delete[] pCtx;
  delete[] pInBuffer;
  delete[] pOutBuffer;

  pCtx = NULL;
  pInBuffer = NULL;
  pOutBuffer = NULL;

  return true;
}

} // namespace ffx
