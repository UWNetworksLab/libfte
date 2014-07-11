/*
 * References:
 *   [FTE1] http://eprint.iacr.org/2012/494.pdf
 *   [FTE2] (to appear summer 2014)
 */

#include "fte.h"

#include <math.h>

#include "src/fte/encrypting/ffx/ffx.h"
#include "src/fte/encrypting/rabbit/rabbit.h"

#include "fte/ranking/dfa_ranker.h"
#include "fte/ranking/dot_star_ranker.h"
#include "fte/ranking/dot_plus_ranker.h"

namespace fte {

Fte::Fte() {
  plaintext_ranker_ = NULL;
  ciphertext_ranker_ = NULL;
  encrypter_ = NULL;
  key_is_set_ = false;
  languages_are_set_ = false;
}

Fte::~Fte() {
  if (plaintext_ranker_ != NULL) {
    if (plaintext_ranker_==ciphertext_ranker_) {
      ciphertext_ranker_ = NULL;
    }
    delete plaintext_ranker_;
  }
  if (ciphertext_ranker_ != NULL) {
    delete ciphertext_ranker_;
  }
  if (encrypter_ != NULL) {
    delete encrypter_;
  }
}

/*
 * Here we set our input/output langauges and verify that the output langauge has
 * capacity at least as large as the input language.
 */
bool Fte::SetLanguages(const std::string & plaintext_dfa,
                       uint32_t plaintext_max_len,
                       const std::string & ciphertext_dfa,
                       uint32_t ciphertext_max_len) {

  if (plaintext_dfa == fte::ranking::DOT_STAR_DFA) {
    plaintext_ranker_ = new ranking::DotStarRanker();
  } else if (plaintext_dfa == fte::ranking::DOT_PLUS_DFA) {
    plaintext_ranker_ = new ranking::DotPlusRanker();
  } else {
    plaintext_ranker_ = new ranking::DfaRanker();
  }
  plaintext_ranker_->SetLanguage(plaintext_dfa, plaintext_max_len);
  plaintext_ranker_->WordsInLanguage(&words_in_plaintext_language_);
  plaintext_language_capacity_in_bits_ = mpz_sizeinbase(words_in_plaintext_language_.get_mpz_t(),
                                         kFfxRadix);

  bool languages_are_the_same = (plaintext_dfa == ciphertext_dfa) && (plaintext_max_len == ciphertext_max_len);
  if (languages_are_the_same) {
    ciphertext_ranker_ = plaintext_ranker_;
  } else {
    if (ciphertext_dfa == fte::ranking::DOT_STAR_DFA) {
      ciphertext_ranker_ = new ranking::DotStarRanker();
    } else if (ciphertext_dfa == fte::ranking::DOT_PLUS_DFA) {
      ciphertext_ranker_ = new ranking::DotPlusRanker();
    } else {
      ciphertext_ranker_ = new ranking::DfaRanker();
    }
    ciphertext_ranker_->SetLanguage(ciphertext_dfa, ciphertext_max_len);
  }

  ciphertext_ranker_->WordsInLanguage(&words_in_ciphertext_language_);
  ciphertext_language_capacity_in_bits_ = mpz_sizeinbase(
      words_in_ciphertext_language_.get_mpz_t(), kFfxRadix);

  if(words_in_plaintext_language_ > words_in_ciphertext_language_) {
    return false;
  }

  languages_are_set_ = true;
  
  // At this point we have a Rabbit implementation that always
  // outperforms Ffx. We may want to enable Ffx in some cases
  // in future.
  // encrypter_ = new fte::encrypting::Ffx(kFfxRadix);
  encrypter_ = new fte::encrypting::Rabbit();
  
  if (key_!="") {
    encrypter_->SetKey(key_);
    key_is_set_ = true;
  }

  return true;
}

bool Fte::set_key(const std::string & key) {
  key_ = key;
  
  if (encrypter_ != NULL) {
    bool success = encrypter_->SetKey(key);
    key_is_set_ = true;
  }
  
  return true;
}

/*
 * This is an implementation of rank-encipher-unrank, as described in [FTE2].
 * We perform cycle walking to ensure that we have a ciphertext in the input
 * domain of the ciphertext ranker.
 */
bool Fte::Encrypt(const std::string & plaintext,
                  std::string * ciphertext) {

  if (!key_is_set_) {
    return false;
  }

  if (!languages_are_set_) {
    return false;
  }

  // ensure ciphertext is initialized to the empty string
  (*ciphertext) = "";

  mpz_class plaintext_rank;
  plaintext_ranker_->Rank(plaintext, &plaintext_rank);
  mpz_class C;
  for (uint32_t j = 0; j < 128; ++j) {
    C = 0;
    int32_t i = j;
    encrypter_->Encrypt(i, plaintext_rank, plaintext_language_capacity_in_bits_, &C);
    while (i > 0 && C >= words_in_ciphertext_language_) {
      --i;
      encrypter_->Encrypt(i, C, plaintext_language_capacity_in_bits_, &C);
    }
    if (i==0 && C < words_in_ciphertext_language_) {
      break;
    }
  }
  ciphertext_ranker_->Unrank(C, ciphertext);

  return true;
}

/*
 * Here we recover a plaintext using rank-decipher-unrank.
 * See [FTE2] for more details.
 */
bool Fte::Decrypt(const std::string & ciphertext,
                  std::string * plaintext) {

  if (!key_is_set_) {
    return false;
  }

  if (!languages_are_set_) {
    return false;
  }

  // ensure plaintext is initialized to the empty string
  (*plaintext) = "";

  mpz_class C;
  ciphertext_ranker_->Rank(ciphertext, &C);
  mpz_class plaintext_rank = 0;
  int32_t i = 0;
  encrypter_->Decrypt(i, C, plaintext_language_capacity_in_bits_, &plaintext_rank);
  while (plaintext_rank >= words_in_plaintext_language_) {
    ++i;
    encrypter_->Decrypt(i, plaintext_rank, plaintext_language_capacity_in_bits_, &plaintext_rank);
    
  }
  plaintext_ranker_->Unrank(plaintext_rank, plaintext);

  return true;
}

} // namespace fte
