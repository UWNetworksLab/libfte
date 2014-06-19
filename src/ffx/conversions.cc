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

  return true;
}

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       unsigned char * out) {

  size_t written_bytes = 0;
  mpz_export(out,             // void *rop
             &written_bytes,  // size_t *countp
             1,               // int order
             1,               // size_t size
             1,               // int endian
             0,               // size_t nails
             in.get_mpz_t()); // const mpz_t op

  uint32_t delta = out_len_in_bytes - written_bytes;
  if (delta > 0) {
    for (int32_t i = out_len_in_bytes-1; i>=delta; --i) {
      out[i] = out[i-delta];
    }
    for (int32_t i = 0; i < delta; i++) {
      out[i] = 0x00;
    }
  }

  return true;
}

bool Base256ToMpzClass(unsigned char * in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out) {

  mpz_import((*out).get_mpz_t(), // mpz_t rop
             in_len_in_bytes,    // size_t count
             1,                  // int order
             1,                  // size_t size
             1,                  // int endian
             0,                  // size_t nails
             in);                // const void *op

  return true;
}

bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out) {

  mpz_class tmp = mpz_class(in, 16);
  MpzClassToBase256(tmp, out_len_in_bytes, out);

  return true;
}

} // namespace ffx
