#include "conversions.h"

namespace ffx {

bool BitMask(const mpz_class & in,
                  uint32_t in_len_in_bits,
                  uint32_t start_bit,
                  uint32_t end_bit,
                  mpz_class * out) {
  (*out) = in;
  (*out) = (*out) >> (in_len_in_bits - end_bit - 1);
  mpz_class modulus;
  mpz_ui_pow_ui(modulus.get_mpz_t(), 2, (end_bit - start_bit + 1));
  (*out) = (*out) % modulus;
}

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out) {
  mpz_class tmp = in;
  for (int32_t i = out_len_in_bytes - 1; i >= 0; --i) {
    mpz_class chunk = tmp & 0xFF;
    out[i] = mpz_get_ui(chunk.get_mpz_t());
    tmp = tmp >> 8;
  }
}

bool Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out) {
  *out = 0;
  for (uint32_t i = 0; i < in_len_in_bytes; ++i) {
    mpz_class chunk = static_cast<uint32_t>(in[i]);
    *out += chunk << (8 * (in_len_in_bytes - 1 - i));
  }
}

bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out) {
  for (uint32_t i = 0; i < out_len_in_bytes; ++i) {
    std::string byte_str = in.substr(i * 2, 2);
    mpz_class chunk = mpz_class(byte_str, 16);
    out[i] = static_cast<unsigned char>(chunk.get_ui());
  }
}

} // namespace ffx
