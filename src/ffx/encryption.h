#ifndef _LIBFTE_SRC_FFX_ENCRYPTION_H
#define _LIBFTE_SRC_FFX_ENCRYPTION_H

#include <gmpxx.h>

#include "key.h"

namespace ffx {

mpz_class aes_ecb( const key, const mpz_class, const uint32_t );
mpz_class aes_cbc_mac( const key, const mpz_class, const uint32_t );

} // namespace ffx

#endif /* _LIBFTE_SRC_FFX_ENCRYPTION_H */