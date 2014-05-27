#ifndef _FFX_CONVERSIONS_H
#define _FFX_CONVERSIONS_H

#include <stdint.h>

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
 * In Python, this is equivalent to:
 *
 *   bin(X).rjust(X_len,'0')[2:][start:end+1]
 *
 */
bool BitMask(const mpz_class & in,
                  uint32_t in_len_in_bits,
                  uint32_t start,
                  uint32_t end,
                  mpz_class * out);

/*
 * mpz_to_base256: Given an mpz_class returns a pointer to a string out
 *                 of length out_length that contains a base-256 representation
 *                 of in. It is the caller's job to determine out_len and initialize
 *                 out.
 * base256_to_mpz: Given a base-256 encoded string of length in_len pointed to
 *                 by in, puts it's base-2 representation in out.
 *                 It is the caller's job to initialize out.
 */

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out);

bool Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out);

/*
 * Given a base-16 encoded string of length in_len pointed to
 * by in, puts It is the callers job to init. out.
 */
bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out);

} // namespace ffx

#endif /* _FFX_CONVERSIONS_H */
