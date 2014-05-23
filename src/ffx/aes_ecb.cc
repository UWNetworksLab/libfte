#include "third_party/aes/aes.h"

#include "ffx/conversions.h"
#include "ffx/aes_ecb.h"

namespace ffx {

mpz_class aes_ecb_encrypt(const Key key,
                          const mpz_class plaintext,
                          const uint32_t plaintext_len) {

  mpz_class retval = 0;
  uint32_t byte_string_len = plaintext_len / 8;
  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pKey = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * pInBuffer = new unsigned char[byte_string_len];
  unsigned char * pOutBuffer = new unsigned char[byte_string_len];

  uint32_t i = 0;
  for(i = 0; i < byte_string_len; i++) {
    pInBuffer[i] = 0;
    pOutBuffer[i] = 0;
  }

  for(i = 0; i < kFFXKeyLengthInBytes; i++) {
    pKey[i] = 0x00;
  }

  mpz_to_base256(plaintext, byte_string_len, pInBuffer);
  base16_to_base256(key.get_key(), kFFXKeyLengthInBytes, pKey);

  aes_init();
  aes_encrypt_key128(pKey, pCtx);
  aes_ecb_encrypt(pInBuffer, pOutBuffer, byte_string_len, pCtx);

  base256_to_mpz(pOutBuffer, byte_string_len, retval);

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
