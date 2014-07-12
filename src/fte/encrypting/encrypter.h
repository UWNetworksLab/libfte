#ifndef _FTE_ENCRYPTER_H
#define _FTE_ENCRYPTER_H

#include <gmpxx.h>

namespace fte {

namespace encrypting {

class Encrypter {
 public:
  virtual bool SetKey(const std::string & key) = 0;

  virtual bool Encrypt(uint32_t step,
                       const mpz_class & plaintext,
                       uint32_t plaintext_len_in_bits,
                       mpz_class * ciphertext) = 0;

  virtual bool Encrypt(uint32_t step,
                       const mpz_class & tweak,
                       uint32_t tweak_len_in_bits,
                       const mpz_class & plaintext,
                       uint32_t plaintext_len_in_bits,
                       mpz_class * ciphertext) = 0;


  virtual bool Decrypt(uint32_t step,
                       const mpz_class & ciphertext,
                       uint32_t ciphertext_len_in_bits,
                       mpz_class * plaintext) = 0;

  virtual bool Decrypt(uint32_t step,
                       const mpz_class & tweak,
                       uint32_t tweak_len_in_bits,
                       const mpz_class & ciphertext,
                       uint32_t ciphertext_len_in_bits,
                       mpz_class * plaintext) = 0;
};

} // namespace encrypting

} // namespace fte

#endif /* _FTE_ENCRYPTER_H */
