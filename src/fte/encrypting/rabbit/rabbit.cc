#include "rabbit.h"

#include <iostream>

#include "fte/encrypting/ffx/conversions.h"

namespace fte {
    
namespace encrypting {

Rabbit::Rabbit() {
  
  uint32_t keysize = 16;
  uint32_t ivsize = 8;
  uint8_t key[keysize];
  std::memset(key, 0, keysize);
  
  cryptor_.keysetup(key, keysize, ivsize);
}
  
bool Rabbit::SetKey(const std::string & key) {  
  if (key.size() != kRabbitKeyLengthInNibbles) {
    return false;
  }
  return true;
}

bool Rabbit::Encrypt(uint32_t step,
                     const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext){

  uint32_t msglen = (plaintext_len_in_bits + 7) / 8;
  uint8_t * input_plaintext = new uint8_t[msglen];
  MpzClassToBase256(plaintext, msglen, input_plaintext);
  //
  uint32_t ivsize = 8;
  uint8_t iv[ivsize];
  std::memset(iv, step, ivsize);
  cryptor_.ivsetup(iv);
  //
  uint8_t * output_ciphertext = new uint8_t[msglen];
  
  cryptor_.encrypt_bytes(input_plaintext, output_ciphertext, msglen);

  Base256ToMpzClass(output_ciphertext, msglen, ciphertext);
  
  delete[] input_plaintext;
  delete[] output_ciphertext;
  
  return true;
}

bool Rabbit::Encrypt(uint32_t step,
                     const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & plaintext,
                     uint32_t plaintext_len_in_bits,
                     mpz_class * ciphertext){
  return Encrypt(step, plaintext, plaintext_len_in_bits, ciphertext);
}


bool Rabbit::Decrypt(uint32_t step,
                     const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext){

  uint32_t msglen = (ciphertext_len_in_bits + 7) / 8;
          
  uint8_t * input_ciphertext = new uint8_t[msglen];
  MpzClassToBase256(ciphertext, msglen, input_ciphertext);
  
  uint8_t * output_plaintext = new uint8_t[msglen];
  //
  uint32_t ivsize = 8;
  uint8_t iv[ivsize];
  std::memset(iv, step, ivsize);
  cryptor_.ivsetup(iv);
  //
  cryptor_.decrypt_bytes(input_ciphertext,
                         output_plaintext, msglen);
  
  Base256ToMpzClass(output_plaintext, msglen, plaintext);
  
  delete[] input_ciphertext;
  delete[] output_plaintext;
  
  return true;
}

bool Rabbit::Decrypt(uint32_t step,
                     const mpz_class & tweak,
                     uint32_t tweak_len_in_bits,
                     const mpz_class & ciphertext,
                     uint32_t ciphertext_len_in_bits,
                     mpz_class * plaintext){
  return Decrypt(step, ciphertext, ciphertext_len_in_bits, plaintext);
}

} // namespace encrypting

} // namespace fte
