#ifndef _LIBFTE_SRC_FFX_CONVERSIONS_H
#define _LIBFTE_SRC_FFX_CONVERSIONS_H

#include <gmpxx.h>

namespace ffx {

mpz_class extract_bit_range( const mpz_class, const uint32_t, const uint32_t, const uint32_t );
void mpz_to_char_array( const mpz_class, const uint32_t, unsigned char * &);
void char_array_to_mpz( unsigned char *, const uint32_t, mpz_class & );
void string_to_mpz( std::string, const uint32_t, mpz_class & );
void fte_key_to_char_array( std::string, const uint32_t, unsigned char * & );

} // namespace ffx

#endif /* _LIBFTE_SRC_FFX_CONVERSIONS_H */