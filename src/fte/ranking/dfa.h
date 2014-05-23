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

#include "fte/exceptions.h"

namespace fte {

namespace ranking {

typedef std::vector<char> array_type_char_t1;
typedef std::vector<bool> array_type_bool_t1;
typedef std::vector<uint32_t> array_type_uint32_t1;
typedef std::vector<mpz_class> array_type_mpz_t1;
typedef std::vector< std::vector<uint32_t> > array_type_uint32_t2;
typedef std::vector< std::vector<mpz_class> > array_type_mpz_t2;
typedef std::vector< std::string > array_type_string_t1;

class DFA {

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
  array_type_uint32_t1 symbols_;

  // our mapping between integers and the symbols in our alphabet; ints -> chars
  std::map<uint32_t, char> sigma_;

  // the reverse mapping of sigma, chars -> ints
  std::map<char, uint32_t> sigma_reverse_;

  // the states in our ranker
  array_type_uint32_t1 states_;

  // our transitions table
  array_type_uint32_t2 delta_;

  // a lookup table used for additional performance gain
  // for each state we detect if all outgoing transitions are to the same state
  array_type_bool_t1 delta_dense_;

  // the set of final states in our ranker
  array_type_uint32_t1 final_states_;

  // buildTable builds a mapping from [q, i] -> n
  //   q: a state in our ranker
  //   i: an integer
  //   n: the number of words in our language that have a path to a final
  //      state that is exactly length i
  void _buildTable();

  // Checks the properties of our ranker, to ensure that we meet all constraints.
  // Throws an exception upon failure.
  void _validate();

  // _T is our cached table, the output of buildTable
  // For a state q and integer i, the value _T[q][i] is the number of unique
  // accepting paths of length exactly i from state q.
  array_type_mpz_t2 CachedTable_;

  mpz_class calculateNumWordsInLanguage( const uint32_t, const uint32_t );
  array_type_mpz_t1 words_in_language_inclusive_;
  array_type_mpz_t1 words_in_language_exclusive_;

 public:
  DFA() {};

  // The constructor of our rank/urank ranker class
  DFA( const std::string, const uint32_t );

  // our unrank function an int -> str mapping
  // given an integer i, return the ith lexicographically ordered string in
  // the language accepted by the ranker
  std::string unrank( const mpz_class );

  // our rank function performs the inverse operation of unrank
  mpz_class rank( const std::string );

  // given integers [n,m] returns the number of words accepted by the
  // ranker that are at least length n and no greater than length m
  mpz_class getNumWordsInLanguage( const uint32_t );
  mpz_class getNumWordsInLanguage( const uint32_t, const uint32_t );
};

} // namespace ranking

} // namespace fte

#endif /* _FTE_RANKING_DFA_H */
