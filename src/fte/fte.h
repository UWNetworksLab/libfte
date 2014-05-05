#ifndef _LIBFTE_SRC_FTE_ENCRYPTER_H
#define _LIBFTE_SRC_FTE_ENCRYPTER_H

#include <iostream>
#include <string.h>

#include "ffx/ffx.h"
#include "fte/ranker.h"
#include "fte/exceptions.h"

namespace fte {

class key : public ffx::key {
public:
    key() : ffx::key() {};
    key( std::string key ) : ffx::key(key) {};
    key( const char * key ) : ffx::key(key) {};
};

class fte {
  private:
    ranker _input_ranker;
    ranker _output_ranker;
    key _key;
    ffx::ffx _ffx;
    mpz_class _words_in_input_language;
    mpz_class _words_in_output_language;
    uint32_t _input_language_capacity;
    uint32_t _output_language_capacity;
  public:
    fte( const std::string, const uint32_t,
               const std::string, const uint32_t,
               const key );
    std::string encrypt( const std::string );
    std::string decrypt( const std::string );

};

} // namespace fte

#endif /* _LIBFTE_SRC_FTE_ENCRYPTER_H */
