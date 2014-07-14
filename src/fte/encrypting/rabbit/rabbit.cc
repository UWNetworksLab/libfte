#include "rabbit.h"

#include <iostream>

#include "fte/encrypting/ffx/conversions.h"

namespace fte {

namespace encrypting {

bool Rabbit::SetKey(const std::string & key) {
  if (key.size() != kRabbitKeyLengthInNibbles) {
    return false;
  }

  uint8_t key_as_uint8[kRabbitKeyLengthInBytes];
  Base16ToBase256(key, kRabbitKeyLengthInBytes, key_as_uint8);

  cryptor_.keysetup(key_as_uint8,
                    kRabbitKeyLengthInBytes, kRabbitIvLengthInBytes);

  return true;
}

bool Rabbit::Encrypt(const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext) {

  uint32_t msglen = (plaintext_len_in_bits + 7) / 8;
  uint8_t * input_plaintext = new uint8_t[msglen];
  MpzClassToBase256(plaintext, msglen, input_plaintext);
  //
  uint32_t ivsize = 8;
  uint8_t iv[ivsize];
  MpzClassToBase256(0, ivsize, iv);
  cryptor_.ivsetup(iv);
  //
  uint8_t * output_ciphertext = new uint8_t[msglen];

  cryptor_.encrypt_bytes(input_plaintext, output_ciphertext, msglen);

  Base256ToMpzClass(output_ciphertext, msglen, ciphertext);

  mpz_class modulus;
  mpz_class base = 2;
  mpz_pow_ui(modulus.get_mpz_t(), base.get_mpz_t(), plaintext_len_in_bits);
  (*ciphertext) = (*ciphertext) % modulus;

  delete[] input_plaintext;
  delete[] output_ciphertext;

  return true;
}

bool Rabbit::Encrypt(const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext) {
  return Encrypt(plaintext, plaintext_len_in_bits, ciphertext);
}


bool Rabbit::Decrypt(const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext) {

  uint32_t msglen = (ciphertext_len_in_bits + 7) / 8;

  uint8_t * input_ciphertext = new uint8_t[msglen];
  MpzClassToBase256(ciphertext, msglen, input_ciphertext);

  uint8_t * output_plaintext = new uint8_t[msglen];
  //
  uint32_t ivsize = 8;
  uint8_t iv[ivsize];
  MpzClassToBase256(0, ivsize, iv);
  cryptor_.ivsetup(iv);
  //
  cryptor_.decrypt_bytes(input_ciphertext,
                         output_plaintext, msglen);

  Base256ToMpzClass(output_plaintext, msglen, plaintext);

  mpz_class modulus;
  mpz_class base = 2;
  mpz_pow_ui(modulus.get_mpz_t(), base.get_mpz_t(), ciphertext_len_in_bits);
  (*plaintext) = (*plaintext) % modulus;

  delete[] input_ciphertext;
  delete[] output_plaintext;

  return true;
}

bool Rabbit::Decrypt(const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext) {
  return Decrypt(ciphertext, ciphertext_len_in_bits, plaintext);
}

} // namespace encrypting

} // namespace fte
