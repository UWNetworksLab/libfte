#include <math.h>

#include "exceptions.h"
#include "ranker.h"
#include "encrypter.h"
#include "ffx2.h"

namespace fte {


encrypter::encrypter( const std::string input_dfa, const uint32_t input_max_len,
                      const std::string output_dfa, const uint32_t output_max_len,
                      const fte::key key ) {
    
    if (key.length() != 32) {
        throw fte::InvalidKeyLength();
    }
    
    _input_ranker = ranker(input_dfa, input_max_len);
    _output_ranker = ranker(output_dfa, output_max_len);
    _key = key;
    _ffx = fte::ffx2::ffx2();

    // validate that _input/_output rankers are compatible
    _words_in_input_language = _input_ranker.getNumWordsInLanguage(input_max_len);
    _words_in_output_language = _output_ranker.getNumWordsInLanguage(output_max_len);
    if ( _words_in_input_language > _words_in_output_language ) {
        throw fte::FteException();
    }

    _input_language_capacity = mpz_sizeinbase(_words_in_input_language.get_mpz_t(),2);
    _output_language_capacity = mpz_sizeinbase(_words_in_output_language.get_mpz_t(),2);
}

std::string encrypter::encrypt( const std::string plaintext ) {
    mpz_class plaintext_rank = _input_ranker.rank(plaintext);
    mpz_class C = _ffx.encrypt( _key, plaintext_rank, _output_language_capacity );
    std::string retval = _output_ranker.unrank(C);
    return retval;
}

std::string encrypter::decrypt( const std::string ciphertext ) {
    mpz_class C = _output_ranker.rank(ciphertext);
    mpz_class plaintext_rank = _ffx.decrypt( _key, C, _output_language_capacity );
    std::string retval = _input_ranker.unrank(plaintext_rank);
    return retval;
}
} // namespace fte
