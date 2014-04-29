#include "ranker.h"

#include <assert.h>
#include <sstream>

/*
 * Please see ranker.h for a detailed explantion of the
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
 *   dfa_str: a minimized ATT FST formatted ranker, see: http://www2.research.att.com/~fsmtools/fsm/man4/fsm.5.html
 *   max_len: the maxium length to compute ranker::buildTable
 */
ranker::ranker(const std::string dfa_str, const uint32_t max_len)
    : _fixed_slice(max_len),
      _start_state(0),
      _num_states(0),
      _num_symbols(0)
{
    // construct the _start_state, _final_states and symbols/states of our ranker
    bool startStateIsntSet = true;
    std::string line;
    std::istringstream my_str_stream(dfa_str);
    while ( getline (my_str_stream,line) )
    {
        if (line.empty()) break;

        array_type_string_t1 split_vec = tokenize( line, '\t' );
        if (split_vec.size() == 4) {
            uint32_t current_state = strtol(split_vec.at(0).c_str(),NULL,10);
            uint32_t new_state = strtol(split_vec.at(1).c_str(),NULL,10);
            uint32_t symbol = strtol(split_vec.at(2).c_str(),NULL,10);

            if (find(_states.begin(), _states.end(), current_state)==_states.end()) {
                _states.push_back( current_state );
            }
            if (find(_states.begin(), _states.end(), new_state)==_states.end()) {
                _states.push_back( new_state );
            }
            if (find(_symbols.begin(), _symbols.end(), symbol)==_symbols.end()) {
                _symbols.push_back( symbol );
            }

            if ( startStateIsntSet ) {
                _start_state = current_state;
                startStateIsntSet = false;
            }
        } else if (split_vec.size()==1) {
            uint32_t final_state = strtol(split_vec.at(0).c_str(),NULL,10);
            if (find(_final_states.begin(), _final_states.end(), final_state)==_final_states.end()) {
                _final_states.push_back( final_state );
            }
        } else if (split_vec.size()>0) {
            throw _InvalidFstFormat;
        } else {
            // blank line, ignore
        }

    }
    _states.push_back( _states.size() ); // extra for the "dead" state

    _num_symbols = _symbols.size();
    _num_states = _states.size();

    // build up our sigma/sigma_reverse tables which enable mappings between
    // bytes/integers
    uint32_t j, k;
    for (j=0; j<_num_symbols; j++) {
        _sigma.insert( std::pair<uint32_t,char>( j, (char)(_symbols.at(j))) );
        _sigma_reverse.insert( std::pair<char,uint32_t>((char)(_symbols.at(j)), j) );
    }

    // initialize all transitions in our ranker to our dead state
    _delta.resize(_num_states);
    for (j=0; j<_num_states; j++) {
        _delta.at(j).resize(_num_symbols);
        for (k=0; k < _num_symbols; k++) {
            _delta.at(j).at(k) = _num_states - 1;
        }
    }

    // fill our our transition function delta
    std::istringstream my_str_stream2(dfa_str);
    while ( getline (my_str_stream2,line) )
    {
        array_type_string_t1 split_vec = tokenize( line, '\t' );
        if (split_vec.size() == 4) {
            uint32_t current_state = strtol(split_vec.at(0).c_str(),NULL,10);
            uint32_t symbol = strtol(split_vec.at(2).c_str(),NULL,10);
            uint32_t new_state = strtol(split_vec.at(1).c_str(),NULL,10);

            symbol = _sigma_reverse.at(symbol);

            _delta.at(current_state).at(symbol) = new_state;
        }
    }

    _delta_dense.resize(_num_states);
    uint32_t q, a;
    for (q=0; q < _num_states; q++ ) {
        _delta_dense.at(q) = true;
        for (a=1; a < _num_symbols; a++) {
            if (_delta.at(q).at(a-1) != _delta.at(q).at(a)) {
                _delta_dense.at(q) = false;
                break;
            }
        }
    }



    ranker::_validate();

    // perform our precalculation to speed up (un)ranking
    ranker::_buildTable();

    if (1 >= getNumWordsInLanguage(0, _fixed_slice)) {
        throw _InvalidInputNoAcceptingPaths;
    }
}


void ranker::_validate() {
    // ensure ranker has at least one state
    if (0 == _states.size())
        throw _InvalidFstFormat;
    if (0 == _final_states.size())
        throw _InvalidFstFormat;

    // ensure ranker has at least one symbol
    if (0 == _sigma.size())
        throw _InvalidFstFormat;
    if (0 == _sigma_reverse.size())
        throw _InvalidFstFormat;

    // ensure we have N states, labeled 0,1,..N-1
    array_type_uint32_t1::iterator state;
    for (state=_states.begin(); state!=_states.end(); state++) {
        if (*state >= _states.size()) {
            throw _InvalidFstStateName;
        }
    }

    // ensure all symbols are in the range 0,1,...,255
    for (uint32_t i = 0; i < _symbols.size(); i++) {
        if (_symbols.at(i) > 256) {
            throw _InvalidSymbol;
        }
    }
}

void ranker::_buildTable() {
    uint32_t i;
    uint32_t q;
    uint32_t a;

    // ensure our table _T is the correct size
    _T.resize(_num_states);
    for (q=0; q<_num_states; q++) {
        _T.at(q).resize(_fixed_slice+1);
        for (i=0; i<=_fixed_slice; i++) {
            _T.at(q).at(i) = 0;
        }
    }

    // set all _T.at(q).at(0) = 1 for all states in _final_states
    array_type_uint32_t1::iterator state;
    for (state=_final_states.begin(); state!=_final_states.end(); state++) {
        _T.at(*state).at(0) = 1;
    }

    // walk through our table _T
    // we want each entry _T.at(q).at(i) to contain the number of strings that start
    // from state q, terminate in a final state, and are of length i
    for (i=1; i<=_fixed_slice; i++) {
        for (q=0; q<_delta.size(); q++) {
            for (a=0; a<_delta.at(0).size(); a++) {
                uint32_t state = _delta.at(q).at(a);
                _T.at(q).at(i) += _T.at(state).at(i-1);
            }
        }
    }
}


std::string ranker::unrank( const mpz_class c_in ) {
    std::string retval;

    // walk the ranker subtracting values from c until we have our n symbols
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

    if (n>_fixed_slice) {
        throw _InvalidRankInput;
    }

    uint32_t i = 0;
    uint32_t q = _start_state;
    uint32_t char_cursor = 0;
    uint32_t state = 0;
    mpz_class char_index = 0;
    for (i=1; i<=n; i++) {
        if (_delta_dense.at(q)) {
            // our optimized version, when _delta[q][i] is equal to n for all symbols i
            state = _delta.at(q).at(0);

            // We do the following two lines with a single call
            // to mpz_fdiv_qr, which is much faster.
            // char_index = (c / _T.at(state).at(n-i));
            // c = c % _T.at(state).at(n-i);
            mpz_fdiv_qr( char_index.get_mpz_t(),
                         c.get_mpz_t(),
                         c.get_mpz_t(),
                         _T.at(state).at(n-i).get_mpz_t() );

            char_cursor = char_index.get_ui();
        } else {
            // traditional goldberg-sipser ranking
            char_cursor = 0;
            state = _delta.at(q).at(char_cursor);

            // A call to mpz_cmp is faster than using >= directly.
            // while (c >= _T.at(state).at(n-i)) {
            while (mpz_cmp( c.get_mpz_t(),
                            _T.at(state).at(n-i).get_mpz_t() )>=0) {

                // Much faster to call mpz_sub, than -=.
                // c -= _T.at(state).at(n-i);
                mpz_sub( c.get_mpz_t(),
                         c.get_mpz_t(),
                         _T.at(state).at(n-i).get_mpz_t() );

                char_cursor += 1;
                state =_delta.at(q).at(char_cursor);
            }
        }
        retval += _sigma.at(char_cursor);
        q = state;
    }

    // bail if our last state q is not in _final_states
    if (find(_final_states.begin(),
             _final_states.end(), q)==_final_states.end()) {
        throw _InvalidInputNoAcceptingPaths;
    }

    return retval;
}

mpz_class ranker::rank( const std::string X ) {
    uint32_t n = X.size();
    mpz_class retval = 0;

    // walk the ranker, adding values from _T to c
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t symbol_as_int = 0;
    uint32_t q = _start_state;
    uint32_t state = 0;
    mpz_class tmp = 0;
    for (i=1; i<=n; i++) {
        try {
            symbol_as_int = _sigma_reverse.at(X.at(i-1));
        } catch (std::exception& e) {
            throw _InvalidSymbol;
        }

        if (_delta_dense.at(q)) {
            // our optimized version, when _delta[q][i] is equal to n for all symbols i
            state = _delta.at(q).at(0);

            // Orders of magnitude faster to use mpz_mul_ui,
            // compared to *.
            // tmp = _T.at(state).at(n-i) * symbol_as_int
            mpz_mul_ui( tmp.get_mpz_t(),
                        _T.at(state).at(n-i).get_mpz_t(),
                        symbol_as_int );

            // mpz_add is faster than +=
            //retval += tmp.get_mpz_t();
            mpz_add( retval.get_mpz_t(),
                     retval.get_mpz_t(),
                     tmp.get_mpz_t() );
        } else {
            // traditional goldberg-sipser ranking
            for (j=1; j<=symbol_as_int; j++) {
                state = _delta.at(q).at(j-1);

                // mpz_add is faster than +=
                //retval += _T.at(state).at(n-i);
                mpz_add( retval.get_mpz_t(),
                         retval.get_mpz_t(),
                         _T.at(state).at(n-i).get_mpz_t() );
            }
        }
        q = _delta.at(q).at(symbol_as_int);
    }

    // bail if our last state q is not in _final_states
    if (find(_final_states.begin(),
             _final_states.end(), q)==_final_states.end()) {
        throw _InvalidInputNoAcceptingPaths;
    }

    retval += getNumWordsInLanguage( 0, n-1 );

    return retval;
}

mpz_class ranker::getNumWordsInLanguage( const uint32_t min_word_length,
        const uint32_t max_word_length )
{
    // verify min_word_length <= max_word_length <= _fixed_slice
    assert(0<=min_word_length);
    assert(min_word_length<=max_word_length);
    assert(max_word_length<=_fixed_slice);

    // count the number of words in the language of length
    // at least min_word_length and no greater than max_word_length
    mpz_class num_words = 0;
    for (uint32_t word_length = min_word_length;
            word_length <= max_word_length;
            word_length++) {
        num_words += _T.at(_start_state).at(word_length);
    }
    return num_words;
}
