/*
 * References:
 *   [FTE1] http://eprint.iacr.org/2012/494.pdf
 *   [FTE2] https://kpdyer.com/publications/usenix2014-libfte-preprint.pdf
 */

#include <math.h>

#include "fte/fte.h"
#include "ffx/ffx.h"

namespace fte {

static bool ValidateKey(const std::string & key) {
  if (key.length() != ffx::kFfxKeyLengthInNibbles) {
    return false;
  }
  for (uint32_t i = 0; i < key.length(); ++i) {
    if (isxdigit(key[i])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

Fte::Fte() {
  ffx_ = ffx::Ffx(kFfxRadix);
  key_is_set_ = false;
  languages_are_set_ = false;
}

/*
 * Here we set our input/output langauges and verify that the output langauge has
 * capacity at least as large as the input language.
 */
bool Fte::SetLanguages(const std::string & plaintext_dfa,
                       uint32_t plaintext_max_len,
                       const std::string & ciphertext_dfa,
                       uint32_t ciphertext_max_len) {

  plaintext_ranker_ = ranking::DfaRanker();
  plaintext_ranker_.SetLanguage(plaintext_dfa, plaintext_max_len);
  plaintext_ranker_.WordsInLanguage(&words_in_plaintext_language_);
  plaintext_language_capacity_in_bits_ = mpz_sizeinbase(words_in_plaintext_language_.get_mpz_t(),
                                         kFfxRadix);

  bool languages_are_the_same = (plaintext_dfa == ciphertext_dfa) && (plaintext_max_len == ciphertext_max_len);
  if (languages_are_the_same) {
    ciphertext_ranker_ = plaintext_ranker_;
  } else {
    ciphertext_ranker_ = ranking::DfaRanker();
    ciphertext_ranker_.SetLanguage(ciphertext_dfa, ciphertext_max_len);
  }

  ciphertext_ranker_.WordsInLanguage(&words_in_ciphertext_language_);
  ciphertext_language_capacity_in_bits_ = mpz_sizeinbase(
      words_in_ciphertext_language_.get_mpz_t(), kFfxRadix);

  if(words_in_plaintext_language_ > words_in_ciphertext_language_) {
    return false;
  }

  languages_are_set_ = true;

  return true;
}

bool Fte::set_key(const std::string & key) {
  if (ValidateKey(key)) {
    key_ = key;
    key_is_set_ = true;
  } else {
    return false;
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

  mpz_class plaintext_rank;
  plaintext_ranker_.Rank(plaintext, &plaintext_rank);
  mpz_class C = 0;
  ffx_.Encrypt(key_, plaintext_rank, ciphertext_language_capacity_in_bits_, &C);
  while (C >= words_in_ciphertext_language_) {
    ffx_.Encrypt(key_, C, ciphertext_language_capacity_in_bits_, &C);
  }
  ciphertext_ranker_.Unrank(C, ciphertext);
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

  mpz_class C;
  ciphertext_ranker_.Rank(ciphertext, &C);
  mpz_class plaintext_rank = 0;
  ffx_.Decrypt(key_, C, ciphertext_language_capacity_in_bits_, &plaintext_rank);
  while (plaintext_rank >= words_in_plaintext_language_) {
    ffx_.Decrypt(key_, plaintext_rank, ciphertext_language_capacity_in_bits_, &plaintext_rank);
  }
  plaintext_ranker_.Unrank(plaintext_rank, plaintext);
}

} // namespace fte
