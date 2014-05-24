#include "ffx/aes_cbc_mac.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

bool AesCbcMac(const std::string & key,
               const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext) {

  mpz_class & retval = *ciphertext;

  uint32_t byte_string_len = plaintext_len_in_bits / 8;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pIv = new unsigned char[kFfxKeyLengthInBytes];
  unsigned char * pKey = new unsigned char[kFfxKeyLengthInBytes];
  unsigned char * pInBuffer = new unsigned char[byte_string_len];
  unsigned char * pOutBuffer = new unsigned char[byte_string_len];

  for (uint32_t i = 0; i < byte_string_len; ++i) {
    pInBuffer[i] = 0;
    pOutBuffer[i] = 0;
  }

  for (uint32_t i = 0; i < kFfxKeyLengthInBytes; ++i) {
    pIv[i] = 0x00;
    pKey[i] = 0x00;
  }

  MpzClassToBase256(plaintext, byte_string_len, pInBuffer);
  Base16ToBase256(key, kFfxKeyLengthInBytes, pKey);

  aes_init();
  aes_encrypt_key128(pKey, pCtx);
  aes_cbc_encrypt(pInBuffer, pOutBuffer, byte_string_len, pIv, pCtx);

  Base256ToMpzClass(pOutBuffer, byte_string_len, &retval);

  retval = BitMask(retval,
                   byte_string_len * 8,
                   byte_string_len * 8 - 128,
                   byte_string_len * 8 - 1);

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
