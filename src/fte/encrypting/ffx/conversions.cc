#include "conversions.h"

namespace fte {

namespace encrypting {

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

  // If mpz_export writes less bytes than specified
  // by out_len_in_bytes, then we need to pad out
  // with zeros.
  uint32_t delta = out_len_in_bytes - written_bytes;
  if (delta > 0) {
    memmove(out + delta, out, out_len_in_bytes - delta);
    memset(out, 0, delta);
  }

  return true;
}

bool MpzClassToBase256(const mpz_class & in,
                       uint32_t out_len_in_bytes,
                       std::string * out) {

  unsigned char* out_uc = new unsigned char[out_len_in_bytes];

  bool retval = MpzClassToBase256(in, out_len_in_bytes, out_uc);
  if (retval) {
    out->assign(reinterpret_cast<char *>(out_uc), out_len_in_bytes);
  }

  delete[] out_uc;

  return retval;
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

bool Base256ToMpzClass(const std::string & in,
                       uint32_t in_len_in_bytes,
                       mpz_class * out) {

  unsigned char* in_uc = new unsigned char[in_len_in_bytes];

  memcpy(in_uc, in.c_str(), in_len_in_bytes);
  bool retval = Base256ToMpzClass(in_uc, in_len_in_bytes, out);

  delete[] in_uc;

  return retval;
}

bool Base16ToBase256(const std::string & in,
                     uint32_t out_len_in_bytes,
                     unsigned char * out) {

  mpz_class tmp = mpz_class(in, 16);
  MpzClassToBase256(tmp, out_len_in_bytes, out);

  return true;
}

} // namespace encrypting

} // namespace fte
