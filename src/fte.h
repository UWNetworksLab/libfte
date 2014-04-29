#ifndef _FTE_H
#define _FTE_H

#include <iostream>
#include <string.h>

class fte {
public:
    fte( std::string dfa, std::string key );
    std::string encrypt( std::string plaintext );
    std::string decrypt( std::string ciphertext );
};

#endif /* _FTE_H */
