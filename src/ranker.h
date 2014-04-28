// This file is part of libfte.
//
// libfte is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libfte is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libfte.  If not, see <http://www.gnu.org/licenses/>.


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


#ifndef _RANK_UNRANK_H
#define _RANK_UNRANK_H

#include <map>
#include <vector>

#include <stdint.h>
#include <gmpxx.h>

typedef std::vector<char> array_type_char_t1;
typedef std::vector<bool> array_type_bool_t1;
typedef std::vector<uint32_t> array_type_uint32_t1;
typedef std::vector< std::vector<uint32_t> > array_type_uint32_t2;
typedef std::vector< std::vector<mpz_class> > array_type_mpz_t2;
typedef std::vector< std::string > array_type_string_t1;

// Exceptions
static class InvalidRankInput : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid rank input: ensure integer is within the correct range.";
    }
} _InvalidRankInput;

static class InvalidUnrankInput : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid unrank input: ensure string is exactly fixed length sizee.";
    }
} _InvalidUnrankInput;

static class InvalidFstFormat : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid FST format.";
    }
} _InvalidFstFormat;

static class InvalidFstStateName : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid ranker format: ranker has N states, and a state that is not in the range 0,1,...,N-1.";
    }
} _InvalidFstStateName;

static class InvalidSymbolRange : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid ranker format: ranker has symbol that is not in the range 0,1,...,255.";
    }
} _InvalidSymbolRange;


static class InvalidInputNoAcceptingPaths : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Please verify your input, the string does not result in an accepting path in the ranker.";
    }
} _InvalidInputNoAcceptingPaths;

static class InvalidSymbol : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Please verify your input, it contains a symbol not in the sigma of the ranker.";
    }
} _InvalidSymbol;

class ranker {

private:
    // the maximum value for which buildTable is computed
    uint32_t _fixed_slice;

    // our ranker start state
    uint32_t _start_state;

    // the number of states in our ranker
    uint32_t _num_states;

    // the number of symbols in our ranker alphabet
    uint32_t _num_symbols;

    // the symbols of our ranker alphabet
    array_type_uint32_t1 _symbols;

    // our mapping between integers and the symbols in our alphabet; ints -> chars
    std::map<uint32_t, char> _sigma;

    // the reverse mapping of sigma, chars -> ints
    std::map<char, uint32_t> _sigma_reverse;

    // the states in our ranker
    array_type_uint32_t1 _states;

    // our transitions table
    array_type_uint32_t2 _delta;

    // a lookup table used for additional performance gain
    // for each state we detect if all outgoing transitions are to the same state
    array_type_bool_t1 _delta_dense;

    // the set of final states in our ranker
    array_type_uint32_t1 _final_states;

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
    array_type_mpz_t2 _T;

public:
    // The constructor of our rank/urank ranker class
    ranker( const std::string, const uint32_t );

    // our unrank function an int -> str mapping
    // given an integer i, return the ith lexicographically ordered string in
    // the language accepted by the ranker
    std::string unrank( const mpz_class );

    // our rank function performs the inverse operation of unrank
    mpz_class rank( const std::string );

    // given integers [n,m] returns the number of words accepted by the
    // ranker that are at least length n and no greater than length m
    mpz_class getNumWordsInLanguage( const uint32_t, const uint32_t );
};

// given a perl-compatiable regular-expression
// returns a (non-minimized) ATT FST formatted ranker
std::string attFstFromRegex( const std::string );

#endif /* _RANK_UNRANK_H */
