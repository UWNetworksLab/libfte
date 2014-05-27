#include "fte/ranking/dfa_ranker.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace fte {

namespace ranking {

/*
 * Please see dfa.h for a detailed explantion of the
 * methods in this file and their purpose.
 */


// Helper fuction. Given a string and a token, performs a python .split()
// Returns a list of string delimnated on the token
static StringVectorT tokenize( const std::string & line,
                               const char & delim ) {
  StringVectorT retval;

  std::istringstream iss(line);
  std::string fragment;
  while(std::getline(iss, fragment, delim)) {
    retval.push_back(fragment);
  }

  return retval;
}

/*
 * Parameters:
 *   dfa: a minimized ATT FST formatted dfa, see: http://www2.research.att.com/~fsmtools/fsm/man4/fsm.5.html
 *   max_len: the maxium length to compute DFA::buildTable
 */
bool DfaRanker::SetLanguage(const std::string & dfa,
                            uint32_t max_word_length) {

  fixed_slice_ = max_word_length;
  start_state_ = 0;
  num_states_ = 0;
  num_symbols_ = 0;

  // construct the _start_state, _final_states and symbols/states of our dfa
  bool startStateIsntSet = true;
  std::string line;
  std::istringstream my_str_stream(dfa);
  while ( getline (my_str_stream,line) ) {
    if (line.empty()) {
      break;
    }

    StringVectorT split_vec = tokenize(line, '\t');
    if (4 == split_vec.size()) {
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
    } else if (1==split_vec.size()) {
      uint32_t final_state = strtol(split_vec.at(0).c_str(),NULL,10);
      if (find(final_states_.begin(), final_states_.end(), final_state)==final_states_.end()) {
        final_states_.push_back( final_state );
      }
    } else if (split_vec.size()>0) {
      return false;
    } else {
      // blank line, ignore
    }

  }
  states_.push_back( states_.size() ); // extra for the "dead" state

  num_symbols_ = symbols_.size();
  num_states_ = states_.size();

  // build up our sigma/sigma_reverse tables which enable mappings between
  // bytes/integers
  for (uint32_t j=0; j<num_symbols_; ++j) {
    sigma_.insert( std::pair<uint32_t,char>( j, (char)(symbols_.at(j))) );
    sigma_reverse_.insert( std::pair<char,uint32_t>((char)(symbols_.at(j)), j) );
  }

  // initialize all transitions in our dfa to our dead state
  delta_.resize(num_states_);
  for (uint32_t j=0; j<num_states_; ++j) {
    delta_.at(j).resize(num_symbols_);
    for (uint32_t k=0; k < num_symbols_; ++k) {
      delta_.at(j).at(k) = num_states_ - 1;
    }
  }

  // fill our our transition function delta
  std::istringstream my_str_stream2(dfa);
  while (getline (my_str_stream2,line)) {
    StringVectorT split_vec = tokenize( line, '\t' );
    if (4 == split_vec.size()) {
      uint32_t current_state = strtol(split_vec.at(0).c_str(),NULL,10);
      uint32_t symbol = strtol(split_vec.at(2).c_str(),NULL,10);
      uint32_t new_state = strtol(split_vec.at(1).c_str(),NULL,10);

      symbol = sigma_reverse_.at(symbol);

      delta_.at(current_state).at(symbol) = new_state;
    }
  }

  delta_dense_.resize(num_states_);
  for (uint32_t q=0; q < num_states_; ++q ) {
    delta_dense_.at(q) = true;
    for (uint32_t a=1; a < num_symbols_; ++a) {
      if (delta_.at(q).at(a-1) != delta_.at(q).at(a)) {
        delta_dense_.at(q) = false;
        break;
      }
    }
  }

  DfaRanker::SanityCheck();

  // perform our precalculation to speed up (un)ranking
  DfaRanker::PopulateCachedTable();

  words_in_language_inclusive_.resize(fixed_slice_+1);
  words_in_language_exclusive_.resize(fixed_slice_+1);
  for (uint32_t max_word_len = 0; max_word_len <= fixed_slice_; ++max_word_len) {
    mpz_class inclusive_dest, exclusive_dest;
    CalculateNumWordsInLanguage(0, max_word_len, &inclusive_dest);
    CalculateNumWordsInLanguage(max_word_len, max_word_len, &exclusive_dest);
    words_in_language_inclusive_.at(max_word_len) = inclusive_dest;
    words_in_language_exclusive_.at(max_word_len) = exclusive_dest;
  }

  mpz_class total_words_in_language;
  WordsInLanguage(0, fixed_slice_, &total_words_in_language);
  if (1 >= total_words_in_language) {
    return false;
  }

  std::sort(symbols_.begin(),symbols_.end());
  std::sort(states_.begin(),states_.end());
  std::sort(final_states_.begin(),final_states_.end());
}


bool DfaRanker::SanityCheck() {
  // ensure dfa has at least one state
  if (0 == states_.size()) {
    return false;
  }
  if (0 == final_states_.size()) {
    return false;
  }

  // ensure dfa has at least one symbol
  if (0 == sigma_.size()) {
    return false;
  }
  if (0 == sigma_reverse_.size()) {
    return false;
  }

  // ensure we have N states, labeled 0,1,..N-1
  Uint32VectorT::iterator state;
  for (state=states_.begin(); state!=states_.end(); ++state) {
    if (*state >= states_.size()) {
      return false;
    }
  }

  // ensure all symbols are in the range 0,1,...,255
  for (uint32_t i = 0; i < symbols_.size(); ++i) {
    if (symbols_.at(i) > 256) {
      return false;
    }
  }
}

bool DfaRanker::PopulateCachedTable() {

  // ensure our table _T is the correct size
  CachedTable_.resize(num_states_);
  for (uint32_t q=0; q<num_states_; ++q) {
    CachedTable_.at(q).resize(fixed_slice_+1);
    for (uint32_t i=0; i<=fixed_slice_; ++i) {
      CachedTable_.at(q).at(i) = 0;
    }
  }

  // set all _T.at(q).at(0) = 1 for all states in _final_states
  Uint32VectorT::iterator state;
  for (state=final_states_.begin(); state!=final_states_.end(); ++state) {
    CachedTable_.at(*state).at(0) = 1;
  }

  // walk through our table _T
  // we want each entry _T.at(q).at(i) to contain the number of strings that start
  // from state q, terminate in a final state, and are of length i
  for (uint32_t i=1; i<=fixed_slice_; ++i) {
    for (uint32_t q=0; q<delta_.size(); ++q) {
      for (uint32_t a=0; a<delta_.at(0).size(); ++a) {
        uint32_t state = delta_.at(q).at(a);
        CachedTable_.at(q).at(i) += CachedTable_.at(state).at(i-1);
      }
    }
  }

  return true;
}


bool DfaRanker::Unrank(const mpz_class & rank,
                       std::string * word) {

  // walk the dfa subtracting values from c until we have our n symbols
  mpz_class c = rank;
  uint32_t n = 0;
  while (true) {
    mpz_class words_in_slice;
    WordsInLanguage(0, n, &words_in_slice);
    bool c_lt_words_in_slice = (mpz_cmp(c.get_mpz_t(), words_in_slice.get_mpz_t()) < 0);
    if (c_lt_words_in_slice) {
      WordsInLanguage(0, n-1, &words_in_slice);
      mpz_sub(c.get_mpz_t(),
              c.get_mpz_t(),
              words_in_slice.get_mpz_t());
      break;
    }
    ++n;
  }

  if (n>fixed_slice_) {
    return false;
  }

  uint32_t q = start_state_;
  uint32_t char_cursor = 0;
  uint32_t state = 0;
  mpz_class char_index = 0;
  for (uint32_t i=1; i<=n; ++i) {
    if (delta_dense_.at(q)) {
      // our optimized version, when _delta[q][i] is equal to n for all symbols i
      state = delta_.at(q).at(0);

      // We do the following two lines with a single call
      // to mpz_fdiv_qr, which is much faster.
      //   char_index = (c / CachedTable_.at(state).at(n-i));
      //   c = c % CachedTable_.at(state).at(n-i);
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
      while (mpz_cmp(c.get_mpz_t(),
                     CachedTable_.at(state).at(n-i).get_mpz_t() )>=0) {

        // Much faster to call mpz_sub, than -=.
        // c -= _T.at(state).at(n-i);
        mpz_sub(c.get_mpz_t(),
                c.get_mpz_t(),
                CachedTable_.at(state).at(n-i).get_mpz_t() );

        ++char_cursor;
        state =delta_.at(q).at(char_cursor);
      }
    }
    (*word) += sigma_.at(char_cursor);
    q = state;
  }

  // bail if our last state q is not in _final_states
  bool q_in_final_states = std::binary_search(final_states_.begin(),
                           final_states_.end(), q);
  if (!q_in_final_states) {
    return false;
  }
}

bool DfaRanker::Rank(const std::string & word,
                     mpz_class * rank) {
  size_t n = word.size();

  // walk the dfa, adding values from _T to c
  uint32_t symbol_as_int = 0;
  uint32_t q = start_state_;
  uint32_t state = 0;
  mpz_class tmp = 0;
  for (uint32_t i=1; i<=n; ++i) {
    try {
      symbol_as_int = sigma_reverse_.at(word.at(i-1));
    } catch (const std::out_of_range& e) {
      return false;
    }

    if (delta_dense_.at(q)) {
      // our optimized version, when _delta[q][i] is equal to some value state for all symbols i
      state = delta_.at(q).at(0);

      // Orders of magnitude faster to use mpz_mul_ui,
      // compared to:
      // tmp = _T.at(state).at(n-i) * symbol_as_int
      mpz_mul_ui( tmp.get_mpz_t(),
                  CachedTable_.at(state).at(n-i).get_mpz_t(),
                  symbol_as_int );

      // mpz_add is faster than +=
      //retval += tmp.get_mpz_t();
      mpz_add( (*rank).get_mpz_t(),
               (*rank).get_mpz_t(),
               tmp.get_mpz_t() );
    } else {
      // traditional goldberg-sipser ranking
      for (uint32_t j=1; j<=symbol_as_int; ++j) {
        state = delta_.at(q).at(j-1);

        // mpz_add is faster than +=
        //retval += _T.at(state).at(n-i);
        mpz_add( (*rank).get_mpz_t(),
                 (*rank).get_mpz_t(),
                 CachedTable_.at(state).at(n-i).get_mpz_t() );
      }
    }
    q = delta_.at(q).at(symbol_as_int);
  }

  // bail if our last state q is not in _final_states
  bool q_in_final_states = std::binary_search(final_states_.begin(),
                           final_states_.end(), q);
  if (!q_in_final_states) {
    return false;
  }

  mpz_class words_in_language;
  WordsInLanguage(0, n-1, &words_in_language);
  mpz_add( (*rank).get_mpz_t(),
           (*rank).get_mpz_t(),
           words_in_language.get_mpz_t() );

  return true;
}

bool DfaRanker::WordsInLanguage(mpz_class * words_in_language) {
  return WordsInLanguage(0,
                         fixed_slice_,
                         words_in_language);
}

bool DfaRanker::WordsInLanguage(uint32_t max_word_length,
                                mpz_class * words_in_language) {
  return WordsInLanguage(0,
                         max_word_length,
                         words_in_language);
}

bool DfaRanker::WordsInLanguage(uint32_t min_word_length,
                                uint32_t max_word_length,
                                mpz_class * words_in_language) {
  if (0 == min_word_length) {
    (*words_in_language) = words_in_language_inclusive_.at(max_word_length);
  } else if (min_word_length==max_word_length) {
    (*words_in_language) = words_in_language_exclusive_.at(max_word_length);
  } else {
    return false;
  }

  return true;
}

bool DfaRanker::CalculateNumWordsInLanguage( uint32_t min_word_length,
    uint32_t max_word_length,
    mpz_class * words_in_language ) {
  // count the number of words in the language of length
  // at least min_word_length and no greater than max_word_length
  for (uint32_t word_length = min_word_length;
       word_length <= max_word_length;
       ++word_length) {
    (*words_in_language) += CachedTable_.at(start_state_).at(word_length);
  }
  return true;
}

} // namespace ranking

} // namespace fte
