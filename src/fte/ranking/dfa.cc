#include "fte/ranking/dfa.h"

#include <assert.h>

#include <iostream>
#include <exception>
#include <sstream>

namespace fte {

namespace ranking {

/*
 * Please see dfa.h for a detailed explantion of the
 * methods in this file and their purpose.
 */


// Helper fuction. Given a string and a token, performs a python .split()
// Returns a list of string delimnated on the token
array_type_string_t1 tokenize( std::string line, char delim ) {
  array_type_string_t1 retval;

  std::istringstream iss(line);
  std::string fragment;
  while(std::getline(iss, fragment, delim))
    retval.push_back(fragment);

  return retval;
}

/*
 * Parameters:
 *   dfa_str: a minimized ATT FST formatted dfa, see: http://www2.research.att.com/~fsmtools/fsm/man4/fsm.5.html
 *   max_len: the maxium length to compute DFA::buildTable
 */
DFA::DFA(const std::string dfa_str, const uint32_t max_len)
  : fixed_slice_(max_len),
    start_state_(0),
    num_states_(0),
    num_symbols_(0) {
  // construct the _start_state, _final_states and symbols/states of our dfa
  bool startStateIsntSet = true;
  std::string line;
  std::istringstream my_str_stream(dfa_str);
  while ( getline (my_str_stream,line) ) {
    if (line.empty()) break;

    array_type_string_t1 split_vec = tokenize( line, '\t' );
    if (split_vec.size() == 4) {
      uint32_t current_state = strtol(split_vec.at(0).c_str(),NULL,10);
      uint32_t new_state = strtol(split_vec.at(1).c_str(),NULL,10);
      uint32_t symbol = strtol(split_vec.at(2).c_str(),NULL,10);

      if (find(states_.begin(), states_.end(), current_state)==states_.end()) {
        states_.push_back( current_state );
      }
      if (find(states_.begin(), states_.end(), new_state)==states_.end()) {
        states_.push_back( new_state );
      }
      if (find(symbols_.begin(), symbols_.end(), symbol)==symbols_.end()) {
        symbols_.push_back( symbol );
      }

      if ( startStateIsntSet ) {
        start_state_ = current_state;
        startStateIsntSet = false;
      }
    } else if (split_vec.size()==1) {
      uint32_t final_state = strtol(split_vec.at(0).c_str(),NULL,10);
      if (find(final_states_.begin(), final_states_.end(), final_state)==final_states_.end()) {
        final_states_.push_back( final_state );
      }
    } else if (split_vec.size()>0) {
      throw fte::InvalidFstFormat();
    } else {
      // blank line, ignore
    }

  }
  states_.push_back( states_.size() ); // extra for the "dead" state

  num_symbols_ = symbols_.size();
  num_states_ = states_.size();

  // build up our sigma/sigma_reverse tables which enable mappings between
  // bytes/integers
  uint32_t j, k;
  for (j=0; j<num_symbols_; j++) {
    sigma_.insert( std::pair<uint32_t,char>( j, (char)(symbols_.at(j))) );
    sigma_reverse_.insert( std::pair<char,uint32_t>((char)(symbols_.at(j)), j) );
  }

  // initialize all transitions in our dfa to our dead state
  delta_.resize(num_states_);
  for (j=0; j<num_states_; j++) {
    delta_.at(j).resize(num_symbols_);
    for (k=0; k < num_symbols_; k++) {
      delta_.at(j).at(k) = num_states_ - 1;
    }
  }

  // fill our our transition function delta
  std::istringstream my_str_stream2(dfa_str);
  while ( getline (my_str_stream2,line) ) {
    array_type_string_t1 split_vec = tokenize( line, '\t' );
    if (split_vec.size() == 4) {
      uint32_t current_state = strtol(split_vec.at(0).c_str(),NULL,10);
      uint32_t symbol = strtol(split_vec.at(2).c_str(),NULL,10);
      uint32_t new_state = strtol(split_vec.at(1).c_str(),NULL,10);

      symbol = sigma_reverse_.at(symbol);

      delta_.at(current_state).at(symbol) = new_state;
    }
  }

  delta_dense_.resize(num_states_);
  uint32_t q, a;
  for (q=0; q < num_states_; q++ ) {
    delta_dense_.at(q) = true;
    for (a=1; a < num_symbols_; a++) {
      if (delta_.at(q).at(a-1) != delta_.at(q).at(a)) {
        delta_dense_.at(q) = false;
        break;
      }
    }
  }

  DFA::_validate();

  // perform our precalculation to speed up (un)ranking
  DFA::_buildTable();

  uint32_t max_word_len;
  words_in_language_inclusive_.resize(fixed_slice_+1);
  words_in_language_exclusive_.resize(fixed_slice_+1);
  for (max_word_len = 0; max_word_len <= fixed_slice_; max_word_len++) {
    words_in_language_inclusive_.at(max_word_len) = calculateNumWordsInLanguage( 0, max_word_len );
    words_in_language_exclusive_.at(max_word_len) = calculateNumWordsInLanguage( max_word_len, max_word_len );
  }

  if (1 >= getNumWordsInLanguage(0, fixed_slice_)) {
    throw fte::InvalidInputNoAcceptingPaths();
  }
}


void DFA::_validate() {
  // ensure dfa has at least one state
  if (0 == states_.size())
    throw fte::InvalidFstFormat();
  if (0 == final_states_.size())
    throw fte::InvalidFstFormat();

  // ensure dfa has at least one symbol
  if (0 == sigma_.size())
    throw fte::InvalidFstFormat();
  if (0 == sigma_reverse_.size())
    throw fte::InvalidFstFormat();

  // ensure we have N states, labeled 0,1,..N-1
  array_type_uint32_t1::iterator state;
  for (state=states_.begin(); state!=states_.end(); state++) {
    if (*state >= states_.size()) {
      throw fte::InvalidFstStateName();
    }
  }

  // ensure all symbols are in the range 0,1,...,255
  for (uint32_t i = 0; i < symbols_.size(); i++) {
    if (symbols_.at(i) > 256) {
      throw fte::InvalidSymbol();
    }
  }
}

void DFA::_buildTable() {
  uint32_t i;
  uint32_t q;
  uint32_t a;

  // ensure our table _T is the correct size
  CachedTable_.resize(num_states_);
  for (q=0; q<num_states_; q++) {
    CachedTable_.at(q).resize(fixed_slice_+1);
    for (i=0; i<=fixed_slice_; i++) {
      CachedTable_.at(q).at(i) = 0;
    }
  }

  // set all _T.at(q).at(0) = 1 for all states in _final_states
  array_type_uint32_t1::iterator state;
  for (state=final_states_.begin(); state!=final_states_.end(); state++) {
    CachedTable_.at(*state).at(0) = 1;
  }

  // walk through our table _T
  // we want each entry _T.at(q).at(i) to contain the number of strings that start
  // from state q, terminate in a final state, and are of length i
  for (i=1; i<=fixed_slice_; i++) {
    for (q=0; q<delta_.size(); q++) {
      for (a=0; a<delta_.at(0).size(); a++) {
        uint32_t state = delta_.at(q).at(a);
        CachedTable_.at(q).at(i) += CachedTable_.at(state).at(i-1);
      }
    }
  }
}


std::string DFA::unrank( const mpz_class c_in ) {
  assert(c_in < getNumWordsInLanguage(0, fixed_slice_));

  std::string retval;

  // walk the dfa subtracting values from c until we have our n symbols
  mpz_class c = c_in;
  uint32_t n = 0;
  while (true) {
    mpz_class words_in_slice = getNumWordsInLanguage( n, n );
    if ( words_in_slice <= c ) {
      c -= words_in_slice;
      n += 1;
    } else {
      break;
    }
  }

  if (n>fixed_slice_) {
    throw fte::InvalidRankInput();
  }

  uint32_t i = 0;
  uint32_t q = start_state_;
  uint32_t char_cursor = 0;
  uint32_t state = 0;
  mpz_class char_index = 0;
  for (i=1; i<=n; i++) {
    if (delta_dense_.at(q)) {
      // our optimized version, when _delta[q][i] is equal to n for all symbols i
      state = delta_.at(q).at(0);

      // We do the following two lines with a single call
      // to mpz_fdiv_qr, which is much faster.
      // char_index = (c / _T.at(state).at(n-i));
      // c = c % _T.at(state).at(n-i);
      mpz_fdiv_qr( char_index.get_mpz_t(),
                   c.get_mpz_t(),
                   c.get_mpz_t(),
                   CachedTable_.at(state).at(n-i).get_mpz_t() );

      char_cursor = char_index.get_ui();
    } else {
      // traditional goldberg-sipser ranking
      char_cursor = 0;
      state = delta_.at(q).at(char_cursor);

      // A call to mpz_cmp is faster than using >= directly.
      // while (c >= _T.at(state).at(n-i)) {
      while (mpz_cmp( c.get_mpz_t(),
                      CachedTable_.at(state).at(n-i).get_mpz_t() )>=0) {

        // Much faster to call mpz_sub, than -=.
        // c -= _T.at(state).at(n-i);
        mpz_sub( c.get_mpz_t(),
                 c.get_mpz_t(),
                 CachedTable_.at(state).at(n-i).get_mpz_t() );

        char_cursor += 1;
        state =delta_.at(q).at(char_cursor);
      }
    }
    retval += sigma_.at(char_cursor);
    q = state;
  }

  // bail if our last state q is not in _final_states
  if (find(final_states_.begin(),
           final_states_.end(), q)==final_states_.end()) {
    throw fte::InvalidInputNoAcceptingPaths();
  }

  return retval;
}

mpz_class DFA::rank( const std::string X ) {
  assert(X.size() <=  fixed_slice_);

  uint32_t n = X.size();
  mpz_class retval = 0;

  // walk the dfa, adding values from _T to c
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t symbol_as_int = 0;
  uint32_t q = start_state_;
  uint32_t state = 0;
  mpz_class tmp = 0;
  for (i=1; i<=n; i++) {
    try {
      symbol_as_int = sigma_reverse_.at(X.at(i-1));
    } catch (std::exception& e) {
      throw fte::InvalidSymbol();
    }

    if (delta_dense_.at(q)) {
      // our optimized version, when _delta[q][i] is equal to n for all symbols i
      state = delta_.at(q).at(0);

      // Orders of magnitude faster to use mpz_mul_ui,
      // compared to *.
      // tmp = _T.at(state).at(n-i) * symbol_as_int
      mpz_mul_ui( tmp.get_mpz_t(),
                  CachedTable_.at(state).at(n-i).get_mpz_t(),
                  symbol_as_int );

      // mpz_add is faster than +=
      //retval += tmp.get_mpz_t();
      mpz_add( retval.get_mpz_t(),
               retval.get_mpz_t(),
               tmp.get_mpz_t() );
    } else {
      // traditional goldberg-sipser ranking
      for (j=1; j<=symbol_as_int; j++) {
        state = delta_.at(q).at(j-1);

        // mpz_add is faster than +=
        //retval += _T.at(state).at(n-i);
        mpz_add( retval.get_mpz_t(),
                 retval.get_mpz_t(),
                 CachedTable_.at(state).at(n-i).get_mpz_t() );
      }
    }
    q = delta_.at(q).at(symbol_as_int);
  }

  // bail if our last state q is not in _final_states
  if (find(final_states_.begin(),
           final_states_.end(), q)==final_states_.end()) {
    throw fte::InvalidInputNoAcceptingPaths();
  }

  retval += getNumWordsInLanguage( 0, n-1 );

  return retval;
}

mpz_class DFA::getNumWordsInLanguage( const uint32_t max_word_length ) {
  return getNumWordsInLanguage( 0, max_word_length );
}

mpz_class DFA::getNumWordsInLanguage( const uint32_t min_word_length,
                                      const uint32_t max_word_length ) {
  mpz_class retval;
  if (min_word_length==0) {
    retval = words_in_language_inclusive_.at(max_word_length);
  } else if (min_word_length==max_word_length) {
    retval = words_in_language_exclusive_.at(max_word_length);
  } else {
    // TODO: throw exception
  }
  return retval;
}

mpz_class DFA::calculateNumWordsInLanguage( const uint32_t min_word_length,
    const uint32_t max_word_length ) {
  // TODO: remove asserts
  // verify min_word_length <= max_word_length <= _fixed_slice
  assert(0<=min_word_length);
  assert(min_word_length<=max_word_length);
  assert(max_word_length<=fixed_slice_);

  // count the number of words in the language of length
  // at least min_word_length and no greater than max_word_length
  mpz_class num_words = 0;
  for (uint32_t word_length = min_word_length;
       word_length <= max_word_length;
       word_length++) {
    num_words += CachedTable_.at(start_state_).at(word_length);
  }
  return num_words;
}

} // namespace ranking

} // namespace fte
