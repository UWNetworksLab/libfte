/*
 * Please see Appendix A of "Protocol Misidentification Made Easy with Format-Transforming Encryption"
 * url: http://dl.acm.org/citation.cfm?id=2516657
 *
 * and
 *
 * "Compression and ranking"
 * url: http://dl.acm.org/citation.cfm?id=22194
 *
 * for details about (un)ranking for regular languages.
 */


#ifndef _FTE_RANKING_DFA_H
#define _FTE_RANKING_DFA_H

#include <stdint.h>

#include <map>
#include <vector>

#include <gmpxx.h>

namespace fte {

namespace ranking {

typedef std::vector<char> CharVectorT;
typedef std::vector<bool> BoolVectorT;
typedef std::vector<uint32_t> Uint32VectorT;
typedef std::vector<mpz_class> MpzClassVectorT;
typedef std::vector< std::string > StringVectorT;

typedef std::vector< Uint32VectorT > Uint32MatrixT;
typedef std::vector< MpzClassVectorT > MpzClassMatrixT;

class DfaRanker {

 private:
  // the maximum value for which buildTable is computed
  uint32_t fixed_slice_;

  // our ranker start state
  uint32_t start_state_;

  // the number of states in our ranker
  uint32_t num_states_;

  // the number of symbols in our ranker alphabet
  uint32_t num_symbols_;

  // the symbols of our ranker alphabet
  Uint32VectorT symbols_;

  // our mapping between integers and the symbols in our alphabet; ints -> chars
  std::map<uint32_t, char> sigma_;

  // the reverse mapping of sigma, chars -> ints
  std::map<char, uint32_t> sigma_reverse_;

  // the states in our ranker
  Uint32VectorT states_;

  // our transitions table
  Uint32MatrixT delta_;

  // a lookup table used for additional performance gain
  // for each state we detect if all outgoing transitions are to the same state
  BoolVectorT delta_dense_;

  // the set of final states in our ranker
  Uint32VectorT final_states_;

  // buildTable builds a mapping from [q, i] -> n
  //   q: a state in our ranker
  //   i: an integer
  //   n: the number of words in our language that have a path to a final
  //      state that is exactly length i
  bool PopulateCachedTable();

  // Checks the properties of our ranker, to ensure that we meet all constraints.
  // Throws an exception upon failure.
  bool SanityCheck();

  // _T is our cached table, the output of buildTable
  // For a state q and integer i, the value _T[q][i] is the number of unique
  // accepting paths of length exactly i from state q.
  MpzClassMatrixT CachedTable_;

  bool CalculateNumWordsInLanguage(uint32_t min_word_len,
                                   uint32_t max_word_len,
                                   mpz_class * words_in_language);
  MpzClassVectorT words_in_language_inclusive_;
  MpzClassVectorT words_in_language_exclusive_;

 public:
  // ...
  bool SetLanguage(const std::string & dfa,
                   uint32_t max_word_length);

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

} // namespace ranking

} // namespace fte

#endif /* _FTE_RANKING_DFA_H */
