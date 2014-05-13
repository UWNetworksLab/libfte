#include "conversions.h"

namespace ffx {

mpz_class extract_bit_range(const mpz_class X, const uint32_t X_len,
                            const uint32_t start, const uint32_t end) {
  mpz_class retval = X;
  retval = retval >> (X_len - end - 1);
  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end - start + 1));
  retval = retval % modulus;
  return retval;
}

void mpz_to_base256(const mpz_class in,
                       const uint32_t out_len,
                       unsigned char * & out) {
  mpz_class tmp = in;
  int32_t i;
  for(i = out_len - 1; i >= 0; i--) {
    mpz_class lowest;
    lowest = tmp % 256;
    uint8_t byteholder = mpz_get_ui(lowest.get_mpz_t());
    out[i] = byteholder;
    tmp = tmp >> 8;
  }
}

void base256_to_mpz(unsigned char * in, const uint32_t in_len,
                       mpz_class & out) {
  out = 0;
  int32_t i = 0;
  for(i = 0; i < in_len; i++) {
    mpz_class nugget = ((uint32_t) in[i]);
    out += nugget << 8 * (in_len - 1 - i);
  }
}

void base16_to_base256(const std::string in,
                       const uint32_t out_len, unsigned char * & out) {
  int32_t i = 0;
  for(i = 0; i < out_len; i++) {
    std::string chunk = in.substr(i * 2, 2);
    mpz_class N = mpz_class(chunk, 16);
    out[i] = (unsigned char) N.get_ui();
  }
}

} // namespace ffx