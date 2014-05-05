#include <math.h>

#include "ffx/ffx.h"

#include "ranking/dfa.h"

#include "fte/exceptions.h"
#include "fte/fte.h"

namespace fte {

fte::fte( const std::string input_dfa, const uint32_t input_max_len,
                      const std::string output_dfa, const uint32_t output_max_len,
                      const key key ) {
    
    if (key.length() != 32) {
        throw InvalidKeyLength();
    }
    
    _input_ranker = ranking::dfa(input_dfa, input_max_len);
    _output_ranker = ranking::dfa(output_dfa, output_max_len);
    _key = key;
    _ffx = ffx::ffx::ffx(2);

    // validate that _input/_output rankers are compatible
    _words_in_input_language = _input_ranker.getNumWordsInLanguage(input_max_len);
    _words_in_output_language = _output_ranker.getNumWordsInLanguage(output_max_len);
    if ( _words_in_input_language > _words_in_output_language ) {
        throw FTEException();
    }

    _input_language_capacity = mpz_sizeinbase(_words_in_input_language.get_mpz_t(),2);
    _output_language_capacity = mpz_sizeinbase(_words_in_output_language.get_mpz_t(),2);
}

std::string fte::encrypt( const std::string plaintext ) {
    // TODO: validate that input plaintext is in input language
    
    mpz_class plaintext_rank = _input_ranker.rank(plaintext);
    mpz_class C = _ffx.encrypt( _key, plaintext_rank, _output_language_capacity );
    std::string retval = _output_ranker.unrank(C);
    return retval;
}

std::string fte::decrypt( const std::string ciphertext ) {
    // TODO: validate that input plaintext is in output language
    
    mpz_class C = _output_ranker.rank(ciphertext);
    mpz_class plaintext_rank = _ffx.decrypt( _key, C, _output_language_capacity );
    std::string retval = _input_ranker.unrank(plaintext_rank);
    return retval;
}

} // namespace fte
