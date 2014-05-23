#include "ffx/aes_ecb.h"

#include "third_party/aes/aes.h"

#include "ffx/ffx.h"
#include "ffx/conversions.h"

namespace ffx {

mpz_class AesEcbEncrypt(const std::string & key,
                        const mpz_class & plaintext,
                        uint32_t plaintext_len_in_bits) {

  mpz_class retval = 0;
  uint32_t byte_string_len = plaintext_len_in_bits / 8;
  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pKey = new unsigned char[kFfxKeyLengthInBytes];
  unsigned char * pInBuffer = new unsigned char[byte_string_len];
  unsigned char * pOutBuffer = new unsigned char[byte_string_len];

  uint32_t i = 0;
  for(i = 0; i < byte_string_len; ++i) {
    pInBuffer[i] = 0;
    pOutBuffer[i] = 0;
  }

  for(i = 0; i < kFfxKeyLengthInBytes; ++i) {
    pKey[i] = 0x00;
  }

  MpzClassToBase256(plaintext, byte_string_len, pInBuffer);
  Base16ToBase256(key, kFfxKeyLengthInBytes, pKey);

  aes_init();
  aes_encrypt_key128(pKey, pCtx);
  aes_ecb_encrypt(pInBuffer, pOutBuffer, byte_string_len, pCtx);

  Base256ToMpzClass(pOutBuffer, byte_string_len, &retval);

  // cleanup
  delete[] pCtx;
  delete[] pKey;
  delete[] pInBuffer;
  delete[] pOutBuffer;

  pCtx = NULL;
  pKey = NULL;
  pInBuffer = NULL;
  pOutBuffer = NULL;

  return retval;
}

} // namespace ffx
