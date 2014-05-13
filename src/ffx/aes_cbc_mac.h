#ifndef _LIBFTE_SRC_FFX_AES_CBC_MAC_H
#define _LIBFTE_SRC_FFX_AES_CBC_MAC_H

#include <gmpxx.h>

#include "ffx/key.h"

namespace ffx {
    
/*
 * On input of plaintext bitstring of length plaintext_len, returns the
 * 128-bit aes_cbc_mac ciphertext of under the provided key. 
 */
mpz_class aes_cbc_mac(const Key key,
                      const mpz_class plaintext,
                      const uint32_t plaintext_len);

} // namespace ffx

#endif /* _LIBFTE_SRC_FFX_AES_CBC_MAC_H */
