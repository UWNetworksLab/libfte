#include <math.h>

#include "ffx/ffx.h"
#include "fte/fte.h"

namespace fte {

FTE::FTE(const std::string input_dfa, const uint32_t input_max_len,
         const std::string output_dfa, const uint32_t output_max_len,
         const Key key) {

  if(key.length() != kFTEKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  input_ranker_ = ranking::dfa(input_dfa, input_max_len);
  output_ranker_ = ranking::dfa(output_dfa, output_max_len);
  key_ = key;
  ffx_ = ffx::FFX(kFFXRadix);

  // validate that _input/_output rankers are compatible
  mpz_class words_in_input_language = input_ranker_.getNumWordsInLanguage(
                                        input_max_len);
  mpz_class words_in_output_language = output_ranker_.getNumWordsInLanguage(
                                         output_max_len);
  if(words_in_input_language > words_in_output_language) {
    throw FTEException();
  }

  input_language_capacity_ = mpz_sizeinbase(words_in_input_language.get_mpz_t(),
                             kFFXRadix);
  output_language_capacity_ = mpz_sizeinbase(
                                words_in_output_language.get_mpz_t(), kFFXRadix);
}

std::string FTE::encrypt(const std::string plaintext) {
  // TODO: validate that input plaintext is in input language

  mpz_class plaintext_rank = input_ranker_.rank(plaintext);
  mpz_class C = ffx_.encrypt(key_, plaintext_rank, output_language_capacity_);
  std::string retval = output_ranker_.unrank(C);
  return retval;
}

std::string FTE::decrypt(const std::string ciphertext) {
  // TODO: validate that input plaintext is in output language

  mpz_class C = output_ranker_.rank(ciphertext);
  mpz_class plaintext_rank = ffx_.decrypt(key_, C, output_language_capacity_);
  std::string retval = input_ranker_.unrank(plaintext_rank);
  return retval;
}

} // namespace fte
