#include <math.h>

#include "fte/fte.h"
#include "ffx/ffx.h"

namespace fte {

Fte::Fte() {
  ffx_ = ffx::Ffx(kFfxRadix);
}

bool Fte::SetLanguages(const std::string & plaintext_dfa,
                       uint32_t plaintext_max_len,
                       const std::string & ciphertext_dfa,
                       uint32_t ciphertext_max_len) {

  plaintext_ranker_ = ranking::DfaRanker();
  plaintext_ranker_.SetLanguage(plaintext_dfa, plaintext_max_len);
  plaintext_ranker_.WordsInLanguage(&words_in_plaintext_language_);
  plaintext_language_capacity_ = mpz_sizeinbase(words_in_plaintext_language_.get_mpz_t(),
                                 kFfxRadix);

  bool languages_are_the_same = (plaintext_dfa == ciphertext_dfa) && (plaintext_max_len == ciphertext_max_len);
  if (languages_are_the_same) {
    ciphertext_ranker_ = plaintext_ranker_;
  } else {
    ciphertext_ranker_ = ranking::DfaRanker(); 
    ciphertext_ranker_.SetLanguage(ciphertext_dfa, ciphertext_max_len);
  }

  ciphertext_ranker_.WordsInLanguage(&words_in_ciphertext_language_);
  ciphertext_language_capacity_ = mpz_sizeinbase(
                                    words_in_ciphertext_language_.get_mpz_t(), kFfxRadix);

  if(words_in_plaintext_language_ > words_in_ciphertext_language_) {
    return false;
  }

  return true;
}

bool Fte::set_key(const std::string & key) {
  if(key.length() == kFteKeyLengthInNibbles) {
    key_ = key;
  } else {
    return false;
  }
  return true;
}

bool Fte::Encrypt(const std::string & plaintext,
                  std::string * ciphertext) {
  // TODO: validate key
  // TODO: validate languages are set

  mpz_class plaintext_rank;
  plaintext_ranker_.Rank(plaintext, &plaintext_rank);
  mpz_class C = 0;
  ffx_.Encrypt(key_, plaintext_rank, ciphertext_language_capacity_, &C);
  while (C >= words_in_ciphertext_language_) {
    ffx_.Encrypt(key_, C, ciphertext_language_capacity_, &C);
  }
  ciphertext_ranker_.Unrank(C, ciphertext);
}

bool Fte::Decrypt(const std::string & ciphertext,
                  std::string * plaintext) {
  // TODO: validate key
  // TODO: validate languages are set

  mpz_class C;
  ciphertext_ranker_.Rank(ciphertext, &C);
  mpz_class plaintext_rank = 0;
  ffx_.Decrypt(key_, C, ciphertext_language_capacity_, &plaintext_rank);
  while (plaintext_rank >= words_in_plaintext_language_) {
    ffx_.Decrypt(key_, plaintext_rank, ciphertext_language_capacity_, &plaintext_rank);
  }
  plaintext_ranker_.Unrank(plaintext_rank, plaintext);
}

} // namespace fte
