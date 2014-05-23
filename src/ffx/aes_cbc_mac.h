#ifndef _FFX_AES_CBC_MAC_H
#define _FFX_AES_CBC_MAC_H

#include <gmpxx.h>

namespace ffx {

/*
 * On input of plaintext bitstring of length plaintext_len, returns the
 * 128-bit aes_cbc_mac ciphertext under the provided key.
 */
mpz_class AesCbcMac(const std::string key,
                      const mpz_class plaintext,
                      const uint32_t plaintext_len);

} // namespace ffx

#endif /* _FFX_AES_CBC_MAC_H */
