#ifndef _FTE_RANKING_DOT_PLUS_H
#define _FTE_RANKING_DOT_PLUS_H

#include <stdint.h>

#include <map>
#include <vector>

#include <gmpxx.h>

#include "fte/ranking/ranker.h"

namespace fte {

namespace ranking {

class DotPlusRanker : public Ranker {

 private:
  uint32_t max_word_len_;

 public:

  // Specify the DFA of the language and the maximum word length that we'll be (un)ranking.
  bool SetLanguage(const std::string & dfa, uint32_t max_word_length);

  // our unrank function an int -> str mapping
  // given an integer i, return the ith lexicographically ordered string in
  // the language accepted by the ranker
  bool Unrank(const mpz_class & rank,
              std::string * word);

  // our rank function performs the inverse operation of unrank
  bool Rank(const std::string & word,
            mpz_class * rank);

  // given integers [n,m] returns the number of words accepted by the
  // ranker that are at least length n and no greater than length m
  bool WordsInLanguage(mpz_class * words_in_language );
  bool WordsInLanguage(uint32_t max_word_length,
                       mpz_class * words_in_language );
  bool WordsInLanguage(uint32_t min_word_length,
                       uint32_t max_word_length,
                       mpz_class * words_in_language );
};

extern std::string DOT_PLUS_DFA;

} // namespace ranking

} // namespace fte

#endif /* _FTE_RANKING_DOT_PLUS_H */
