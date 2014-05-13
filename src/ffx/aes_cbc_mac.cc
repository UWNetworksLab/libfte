#include "aes/aes.h"

#include "ffx/conversions.h"
#include "ffx/aes_cbc_mac.h"

namespace ffx {

mpz_class aes_cbc_mac(const Key key,
                      const mpz_class plaintext,
                      const uint32_t plaintext_len) {
  mpz_class retval = 0;

  uint32_t byte_string_len = plaintext_len / 8;

  aes_encrypt_ctx * pCtx = new aes_encrypt_ctx[1];
  unsigned char * pIv = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * pKey = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * pInBuffer = new unsigned char[byte_string_len];
  unsigned char * pOutBuffer = new unsigned char[byte_string_len];

  uint32_t i = 0;
  for(i = 0; i < byte_string_len; i++) {
    pInBuffer[i] = 0;
    pOutBuffer[i] = 0;
  }

  for(i = 0; i < kFFXKeyLengthInBytes; i++) {
    pIv[i] = 0x00;
    pKey[i] = 0x00;
  }

  mpz_to_base256(plaintext, byte_string_len, pInBuffer);
  base16_to_base256(key.get_key(), kFFXKeyLengthInBytes, pKey);

  aes_init();
  aes_encrypt_key128(pKey, pCtx);
  aes_cbc_encrypt(pInBuffer, pOutBuffer, byte_string_len, pIv, pCtx);

  base256_to_mpz(pOutBuffer, byte_string_len, retval);

  retval = extract_bit_range(retval,
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

  return retval;
}

} // namespace ffx
