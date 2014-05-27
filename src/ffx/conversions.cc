#include "conversions.h"

namespace ffx {

bool BitMask(const mpz_class & in,
                  uint32_t in_len_in_bits,
                  uint32_t start,
                  uint32_t end,
                  mpz_class * out) {
  (*out) = in;
  (*out) = (*out) >> (in_len_in_bits - end - 1);
  mpz_class modulus = 0;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end - start + 1));
  (*out) = (*out) % modulus;
}

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out) {
  mpz_class tmp = in;
  for (int32_t i = out_len_in_bytes - 1; i >= 0; --i) {
    mpz_class lowest;
    lowest = tmp % 256;
    uint8_t byteholder = mpz_get_ui(lowest.get_mpz_t());
    out[i] = byteholder;
    tmp = tmp >> 8;
  }
}

bool Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out) {
  *out = 0;
  for (uint32_t i = 0; i < in_len_in_bytes; ++i) {
    mpz_class chunk = ((uint32_t) in[i]);
    *out += chunk << 8 * (in_len_in_bytes - 1 - i);
  }
}

bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out) {
  for (uint32_t i = 0; i < out_len_in_bytes; ++i) {
    std::string chunk = in.substr(i * 2, 2);
    mpz_class N = mpz_class(chunk, 16);
    out[i] = (unsigned char) N.get_ui();
  }
}

} // namespace ffx
