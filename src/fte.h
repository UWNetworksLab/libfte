#ifndef _FTE_H
#define _FTE_H

#include <iostream>
#include <string.h>

#include "ranker.h"

class fte {
private:
    ranker _input_ranker;
    ranker _output_ranker;
public:
    fte( std::string dfa, uint32_t max_len, std::string key );
    std::string encrypt( std::string plaintext );
    std::string decrypt( std::string ciphertext );

};

extern std::string DOT_STAR;

#endif /* _FTE_H */
