#ifndef _FTE_RANKING_RANKER_H
#define _FTE_RANKING_RANKER_H

namespace fte {

namespace ranking {

class Ranker {

 public:
  // Specify the DFA of the language and the maximum word length that we'll be (un)ranking.
  virtual bool SetLanguage(const std::string & dfa,
                   uint32_t max_word_length) = 0;

  // our unrank function an int -> str mapping
  // given an integer i, return the ith lexicographically ordered string in
  // the language accepted by the ranker
  virtual bool Unrank(const mpz_class & rank,
              std::string * word) = 0;

  // our rank function performs the inverse operation of unrank
  virtual bool Rank(const std::string & word,
            mpz_class * rank) = 0;

  // given integers [n,m] returns the number of words accepted by the
  // ranker that are at least length n and no greater than length m
  virtual bool WordsInLanguage(mpz_class * words_in_language ) = 0;
  virtual bool WordsInLanguage(uint32_t max_word_length,
                       mpz_class * words_in_language ) = 0;
  virtual bool WordsInLanguage(uint32_t min_word_length,
                       uint32_t max_word_length,
                       mpz_class * words_in_language ) = 0;
};

} // namespace ranking

} // namespace fte

#endif /* _FTE_RANKING_RANKER_H */
