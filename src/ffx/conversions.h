#ifndef _LIBFTE_SRC_FFX_CONVERSIONS_H
#define _LIBFTE_SRC_FFX_CONVERSIONS_H

#include <gmpxx.h>

namespace ffx {

/* 
 * Given a bitstring X of length X_len, extract_bit_range returns
 * the bits in index start to end, inclusive.
 * 
 * For example:
 * inputs:
 *  - X = 0b10001000
 *  - X_len = 8
 *  - start = 0, end = 3
 * output
 *  - retval = 0b1000
 *    because  0b10001000
 *               [  ]
 *   position    01234567
 * 
 * For those that love Python, this is equivalent to:
 *   
 *   bin(X).rjust(X_len,'0')[2:][start:end+1]
 * 
 */
mpz_class extract_bit_range(const mpz_class X, const uint32_t X_len,
                            const uint32_t start, const uint32_t end);


void mpz_to_char_array(const mpz_class, const uint32_t, unsigned char * &);


void char_array_to_mpz(unsigned char *, const uint32_t, mpz_class &);


void string_to_mpz(std::string, const uint32_t, mpz_class &);


void fte_key_to_char_array(std::string, const uint32_t, unsigned char * &);

} // namespace ffx

#endif /* _LIBFTE_SRC_FFX_CONVERSIONS_H */