#ifndef _FFX_CONVERSIONS_H
#define _FFX_CONVERSIONS_H

#include <stdint.h>

#include <gmpxx.h>

namespace fte {

namespace encrypting {

/*
 * Given a bitstring in of length in_len_in_bits, BitMask extracts
 * the bits in index start_bit to end_bit, inclusive.
 *
 * For example:
 * inputs:
 *  - in = 0b10001000
 *  - in_len_in_bits = 8
 *  - start_bit = 0, end_bit = 3
 * output
 *  - retval = 0b1000
 *    because  0b10001000
 *               [  ]
 *   position    01234567
 *
 * In Python, this is equivalent to:
 *
 *   out = bin(in)
 *   out = out.rjust(in_len_in_bits,'0')
 *   out = out[2:]
 *   out = out[start_bit:end_bit+1]
 *
 */
bool BitMask(const mpz_class & in,
             uint32_t in_len_in_bits,
             uint32_t start_bit,
             uint32_t end_bit,
             mpz_class * out);

/*
 * MpzClassToBase256: Given an mpz_class returns a pointer to a string out
 *                 of length out_len_in_bytes that contains a base-256 representation
 *                 of in. It is the caller's job to determine out_len_in_bytes and initialize
 *                 out.
 * Base256ToMpzClass: Given a base-256 encoded string of length in_len_in_bytes pointed to
 *                 by in, puts it's base-2 representation in out.
 *                 It is the caller's job to initialize out.
 */

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out);

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       std::string * out);

bool Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out);

bool Base256ToMpzClass(const std::string & in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out);

/*
 * Given a base-16 encoded string of length out_len_in_bytes pointed to
 * by in, puts It is the callers job to init. out.
 */
bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out);

} // namespace encrypting

} // namespace fte

#endif /* _FFX_CONVERSIONS_H */
