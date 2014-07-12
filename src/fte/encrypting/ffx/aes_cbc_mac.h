#ifndef _FFX_AES_CBC_MAC_H
#define _FFX_AES_CBC_MAC_H

#include <stdint.h>

#include <gmpxx.h>

namespace fte {

namespace encrypting {

bool AesCbcSetKey(unsigned char * key);

/*
 * On input of plaintext bitstring of length plaintext_len_in_bits, populates ciphertext with the
 * 128-bit aes_cbc_mac ciphertext under the provided key.
 */
bool AesCbcMac(const mpz_class & plaintext,
               uint32_t plaintext_len_in_bits,
               mpz_class * ciphertext);

} // namespace encrypting

} // namespace fte

#endif /* _FFX_AES_CBC_MAC_H */
