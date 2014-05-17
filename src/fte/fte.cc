#include <math.h>

#include "ffx/ffx.h"
#include "fte/fte.h"

namespace fte {

FTE::FTE(const std::string input_dfa, const uint32_t input_max_len,
         const std::string output_dfa, const uint32_t output_max_len,
         const Key key) {

  // TODO: don't throw an exception in the constructor
  if(key.length() != kFTEKeyLengthInNibbles) {
    throw InvalidKeyLength();
  }

  input_ranker_ = ranking::DFA(input_dfa, input_max_len);
  output_ranker_ = ranking::DFA(output_dfa, output_max_len);
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
  // TODO: catch exceptions from ranker

  mpz_class plaintext_rank = input_ranker_.rank(plaintext);
  std::cout << plaintext_rank.get_str() << std::endl;
  std::cout << "cap: " << output_language_capacity_ << std::endl;
  mpz_class C = ffx_.encrypt(key_, plaintext_rank, output_language_capacity_);
  std::cout << mpz_sizeinbase(C.get_mpz_t(), 2) << std::endl;
  std::string retval = output_ranker_.unrank(C);
  
  return retval;
}

std::string FTE::decrypt(const std::string ciphertext) {
  // TODO: catch exceptions from ranker

  mpz_class C = output_ranker_.rank(ciphertext);
  mpz_class plaintext_rank = ffx_.decrypt(key_, C, output_language_capacity_);
  std::string retval = input_ranker_.unrank(plaintext_rank);
  
  return retval;
}

} // namespace fte
