#ifndef _FFX_AES_ECB_H
#define _FFX_AES_ECB_H

#include <stdint.h>

#include <gmpxx.h>

namespace ffx {

/*
 * On input of plaintext bitstring of length plaintext_len_in_bits, populates ciphertext with the
 * 128-bit aes_ecb ciphertext under the provided key.
 */
bool AesEcbEncrypt(const std::string & key,
                   const mpz_class & plaintext,
                   uint32_t plaintext_len_in_bits,
                   mpz_class * ciphertext);

} // namespace ffx

#endif /* _FFX_AES_ECB_H */
