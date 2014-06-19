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

  aes_encrypt_ctx * ctx = new aes_encrypt_ctx[1];
  unsigned char * in_buffer = new unsigned char[plaintext_len_in_bytes];
  unsigned char * out_buffer = new unsigned char[ciphertext_len_in_bytes];

  memset(in_buffer, 0, plaintext_len_in_bytes);
  memset(out_buffer, 0, plaintext_len_in_bytes);

  MpzClassToBase256(plaintext, plaintext_len_in_bytes, in_buffer);

  aes_init();
  aes_encrypt_key128(key, ctx);
  aes_ecb_encrypt(in_buffer, out_buffer, plaintext_len_in_bytes, ctx);

  Base256ToMpzClass(out_buffer, ciphertext_len_in_bytes, ciphertext);

  // cleanup
  delete[] ctx;
  delete[] in_buffer;
  delete[] out_buffer;

  return true;
}

} // namespace ffx
