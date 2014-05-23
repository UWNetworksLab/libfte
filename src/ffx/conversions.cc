#include "conversions.h"

namespace ffx {

mpz_class BitMask(const mpz_class & in,
                  uint32_t in_len_in_bits,
                  uint32_t start,
                  uint32_t end) {
  mpz_class retval = in;
  retval = retval >> (in_len_in_bits - end - 1);
  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end - start + 1));
  retval = retval % modulus;
  return retval;
}

void MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out) {
  mpz_class tmp = in;
  int32_t i;
  for(i = out_len_in_bytes - 1; i >= 0; --i) {
    mpz_class lowest;
    lowest = tmp % 256;
    uint8_t byteholder = mpz_get_ui(lowest.get_mpz_t());
    out[i] = byteholder;
    tmp = tmp >> 8;
  }
}

void Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out) {
  *out = 0;
  int32_t i = 0;
  for(i = 0; i < in_len_in_bytes; ++i) {
    mpz_class chunk = ((uint32_t) in[i]);
    *out += chunk << 8 * (in_len_in_bytes - 1 - i);
  }
}

void Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes, 
                     unsigned char * out) {
  int32_t i = 0;
  for(i = 0; i < out_len_in_bytes; ++i) {
    std::string chunk = in.substr(i * 2, 2);
    mpz_class N = mpz_class(chunk, 16);
    out[i] = (unsigned char) N.get_ui();
  }
}

} // namespace ffx
