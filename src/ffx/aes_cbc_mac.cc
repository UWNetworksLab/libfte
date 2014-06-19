#include "ffx/aes_cbc_mac.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

bool AesCbcMac(unsigned char * key,
               const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext) {

  uint32_t plaintext_len_in_bytes = (plaintext_len_in_bits + 7) / 8;
  uint32_t ciphertext_len_in_bytes = plaintext_len_in_bytes;
  uint32_t mac_len_in_bytes = 16;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pIv = new unsigned char[kFfxIvLengthInBytes];
  unsigned char * pInBuffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * pOutBuffer = new unsigned char[ciphertext_len_in_bytes];

  memset(pInBuffer, 0, plaintext_len_in_bytes);
  memset(pOutBuffer, 0, plaintext_len_in_bytes);
  memset(pIv, 0, kFfxIvLengthInBytes);

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, pInBuffer);

  aes_init();
  aes_encrypt_key128(key, pCtx);
  aes_cbc_encrypt(pInBuffer, pOutBuffer, plaintext_len_in_bytes, pIv, pCtx);

  unsigned char * mac_offset = pOutBuffer + (ciphertext_len_in_bytes - mac_len_in_bytes);
  Base256ToMpzClass(mac_offset,
                    mac_len_in_bytes,
                    ciphertext);

  // cleanup
  delete[] pCtx;
  delete[] pIv;
  delete[] pInBuffer;
  delete[] pOutBuffer;

  pCtx = NULL;
  pIv = NULL;
  pInBuffer = NULL;
  pOutBuffer = NULL;

  return true;
}

} // namespace ffx
