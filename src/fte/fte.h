/*
 * This class implements a Format-Transforming Encryption scheme for
 * regular languages. The input plaintext language and output ciphertext
 * language can be specified using DFAs.
 *
 * As example example, say we have:
 *   - DFA M1 that accepts the language generated by regex R1 = "^(a|b)+$"
 *   - DFA M2 that accepts the language generated by regex R2 = "^(x|y)+$"
 *
 * Then the FTE[M1,M2] scheme will accepts plaintexts in the language L(R1)
 * and output ciphertexts in the language of L(R2).
 *
 * In the FTE interface we introduce two additional parameters: input_max_len
 * and output_max_len. These are for efficiency purposes and restrict the max.
 * length for any input/output string, respectively.
 */

#ifndef _FTE_FTE_H
#define _FTE_FTE_H

#include <string>

#include "fte/encrypting/encrypter.h"
#include "fte/ranking/ranker.h"

namespace fte {

const uint32_t kFfxRadix = 2;
const uint32_t kFfxRounds = 2;
const uint32_t kMaxCycles = 1024;

class Fte {
 private:
  // our rankers
  ranking::Ranker * plaintext_ranker_;
  ranking::Ranker * ciphertext_ranker_;

  // the key and encrypter
  std::string key_;
  encrypting::Encrypter * encrypter_;

  // the capacity of our plaintext|ciphertext language
  uint32_t plaintext_language_capacity_in_bits_;
  uint32_t ciphertext_language_capacity_in_bits_;

  // the number of actual words in our plaintext|ciphertext langauge
  mpz_class words_in_plaintext_language_;
  mpz_class words_in_ciphertext_language_;

  bool key_is_set_;
  bool languages_are_set_;
  uint32_t max_cycles_;

 public:
  // default constructor
  Fte();
  ~Fte();

  /*
   * Specify the input key.
   */
  bool set_key(const std::string & key);

  /*
   * plaintext_dfa and plaintext_max_len specify the input plaintext language
   * ciphertext_dfa and ciphertext_max_len specificy the output ciphertext language
   * the key is a 32-character hex string string
   */
  bool SetLanguages(const std::string & plaintext_dfa,
                    uint32_t plaintext_max_len,
                    const std::string & ciphertext_dfa,
                    uint32_t ciphertext_max_len);

  /*
   * On input of a string that is accepted by input_dfa (specified in the constructor)
   * the encrypt function outputs a ciphertext that is accepted by output_dfa.
   * The plaintext cannot exceed length input_max_len.
   */
  bool Encrypt(const std::string & plaintext,
               std::string * ciphertext);

  /*
   * On input of a string that is accepted by output_dfa (specified in the constructor)
   * the decrypt function recovers a plaintext.
   */
  bool Decrypt(const std::string & ciphertext,
               std::string * plaintext);

};

} // namespace fte

#endif /* _FTE_FTE_H */
