#ifndef _LIBFTE_SRC_FFX_AES_CBC_MAC_H
#define _LIBFTE_SRC_FFX_AES_CBC_MAC_H

#include <gmpxx.h>

#include "ffx/key.h"

namespace ffx {

mpz_class aes_cbc_mac(const Key key,
                      const mpz_class plaintext,
                      const uint32_t plaintext_len);

} // namespace ffx

#endif /* _LIBFTE_SRC_FFX_AES_CBC_MAC_H */
