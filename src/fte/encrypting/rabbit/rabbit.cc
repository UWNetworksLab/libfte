#include "rabbit.h"

#include <iostream>

#include "fte/encrypting/ffx/conversions.h"

namespace fte {
    
namespace encrypting {

Rabbit::Rabbit() {

}
  
bool Rabbit::SetKey(const std::string & key) {
  uint32_t key_len = key.size() / 2;
  uint8_t * key_str = new uint8_t[key_len];
  Base16ToBase256(key.data(), key_len, key_str);
  
  if (key_len != cryptor_.kDefaultKeySize) {
    return false;
  }
  
  key_.assign(reinterpret_cast<const char*>(key_str), key_len);
  cryptor_.keysetup(reinterpret_cast<const uint8_t*>(key_.data()),
                    key_.size(), cryptor_.kDefaultIvSize);
  
  return true;
}

bool Rabbit::Encrypt(const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext){
  (*ciphertext) = plaintext;
  
  return true;
}

bool Rabbit::Encrypt(const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext){
  return Encrypt(plaintext, plaintext_len_in_bits, ciphertext);
}


bool Rabbit::Decrypt(const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext){
  (*plaintext) = ciphertext;
  
  return true;
}

bool Rabbit::Decrypt(const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext){
  return Decrypt(ciphertext, ciphertext_len_in_bits, plaintext);
}

} // namespace encrypting

} // namespace fte