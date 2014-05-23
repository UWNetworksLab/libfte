#include <math.h>

#include "fte/fte.h"
#include "ffx/ffx.h"

namespace fte {

Fte::Fte(const std::string & input_dfa, uint32_t input_max_len,
         const std::string & output_dfa, uint32_t output_max_len,
         const std::string & key) {

  // TODO: don't throw an exception in the constructor
  if(key.length() != kFteKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  input_ranker_ = ranking::DfaRanker(input_dfa, input_max_len);
  output_ranker_ = ranking::DfaRanker(output_dfa, output_max_len);
  key_ = key;
  ffx_ = ffx::Ffx(kFfxRadix);

  // validate that _input/_output rankers are compatible
  words_in_input_language_ = input_ranker_.WordsInLanguage(
                               input_max_len);
  words_in_output_language_ = output_ranker_.WordsInLanguage(
                                output_max_len);

  if(words_in_input_language_ > words_in_output_language_) {
    throw FteException();
  }

  input_language_capacity_ = mpz_sizeinbase(words_in_input_language_.get_mpz_t(),
                             kFfxRadix);
  output_language_capacity_ = mpz_sizeinbase(
                                words_in_output_language_.get_mpz_t(), kFfxRadix);
}

std::string Fte::Encrypt(const std::string & plaintext) {
  // TODO: catch exceptions from ranker

  mpz_class plaintext_rank = input_ranker_.Rank(plaintext);
  mpz_class C = ffx_.Encrypt(key_, plaintext_rank, output_language_capacity_);
  while (C >= words_in_output_language_) {
    C = ffx_.Encrypt(key_, C, output_language_capacity_);
  }
  std::string retval = output_ranker_.Unrank(C);

  return retval;
}

std::string Fte::Decrypt(const std::string & ciphertext) {
  // TODO: catch exceptions from ranker

  mpz_class C = output_ranker_.Rank(ciphertext);
  mpz_class plaintext_rank = ffx_.Decrypt(key_, C, output_language_capacity_);
  while (plaintext_rank >= words_in_input_language_) {
    plaintext_rank = ffx_.Decrypt(key_, plaintext_rank, output_language_capacity_);
  }
  std::string retval = input_ranker_.Unrank(plaintext_rank);

  return retval;
}

} // namespace fte
