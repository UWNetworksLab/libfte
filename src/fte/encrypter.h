#ifndef _LIBFTE_SRC_FTE_ENCRYPTER_H
#define _LIBFTE_SRC_FTE_ENCRYPTER_H

#include <iostream>
#include <string.h>

#include "exceptions.h"
#include "ranker.h"
#include "ffx2.h"

namespace fte {

class encrypter {
  private:
    ranker _input_ranker;
    ranker _output_ranker;
    std::string _key;
    fte::ffx2 _ffx;
    mpz_class _words_in_input_language;
    mpz_class _words_in_output_language;
    uint32_t _input_language_capacity;
    uint32_t _output_language_capacity;
  public:
    encrypter( const std::string, const uint32_t,
               const std::string, const uint32_t,
               const std::string );
    std::string encrypt( const std::string );
    std::string decrypt( const std::string );

};

} // namespace fte

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
