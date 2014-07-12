#include "aes_cbc_mac.h"

#include "third_party/aes/aes.h"

#include "fte/encrypting/ffx/ffx.h"
#include "fte/encrypting/ffx/conversions.h"

namespace fte {

namespace encrypting {

static aes_encrypt_ctx * ctx = NULL;

bool AesCbcSetKey(unsigned char * key) {
  if (ctx == NULL) {
    ctx = new aes_encrypt_ctx[1];
  }
  aes_encrypt_key128(key, ctx);
  return true;
}

bool AesCbcMac(const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext) {

  if (ctx == NULL) {
    return false;
  }

  uint32_t plaintext_len_in_bytes = (plaintext_len_in_bits + 7) / 8;
  uint32_t ciphertext_len_in_bytes = plaintext_len_in_bytes;
  uint32_t mac_len_in_bytes = 16;

  unsigned char * iv = new unsigned char[kFfxIvLengthInBytes];
  unsigned char * in_buffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * out_buffer = new unsigned char[ciphertext_len_in_bytes];

  memset(in_buffer, 0, plaintext_len_in_bytes);
  memset(out_buffer, 0, plaintext_len_in_bytes);
  memset(iv, 0, kFfxIvLengthInBytes);

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, in_buffer);

  aes_cbc_encrypt(in_buffer, out_buffer, plaintext_len_in_bytes, iv, ctx);

  unsigned char * mac_ptr = out_buffer + (ciphertext_len_in_bytes - mac_len_in_bytes);
  Base256ToMpzClass(mac_ptr,
                    mac_len_in_bytes,
                    ciphertext);

  // cleanup
  delete[] iv;
  delete[] in_buffer;
  delete[] out_buffer;

  return true;
}

} // namespace encrypting

} // namespace fte
