#ifndef _LIBFTE_SRC_FTE_H
#define _LIBFTE_SRC_FTE_H

#include <iostream>
#include <string.h>
#include "exceptions.h"
#include "ranker.h"

namespace fte {

class encrypter {
private:
    ranker _input_ranker;
    ranker _output_ranker;
public:
    encrypter( const std::string, const uint32_t,
         const std::string, const uint32_t,
         const std::string );
    std::string encrypt( const std::string );
    std::string decrypt( const std::string );

};

} // namespace fte

#endif /* _LIBFTE_SRC_FTE_H */
