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
  unsigned char * iv = new unsigned char[kFfxIvLengthInBytes];
  unsigned char * in_buffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * out_buffer = new unsigned char[ciphertext_len_in_bytes];

  memset(in_buffer, 0, plaintext_len_in_bytes);
  memset(out_buffer, 0, plaintext_len_in_bytes);
  memset(iv, 0, kFfxIvLengthInBytes);

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, in_buffer);

  aes_init();
  aes_encrypt_key128(key, pCtx);
  aes_cbc_encrypt(in_buffer, out_buffer, plaintext_len_in_bytes, iv, pCtx);

  unsigned char * mac_offset = out_buffer + (ciphertext_len_in_bytes - mac_len_in_bytes);
  Base256ToMpzClass(mac_offset,
                    mac_len_in_bytes,
                    ciphertext);

  // cleanup
  delete[] pCtx;
  delete[] iv;
  delete[] in_buffer;
  delete[] out_buffer;

  pCtx = NULL;
  iv = NULL;
  in_buffer = NULL;
  out_buffer = NULL;

  return true;
}

} // namespace ffx
