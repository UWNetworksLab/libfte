#ifndef _LIBFTE_SRC_FTE_KEY_H
#define _LIBFTE_SRC_FTE_KEY_H

#include <stdio.h>
#include <iostream>
#include <string.h>

namespace ffx {

class key {
  private:
    std::string _key;
  public:
    key() {};
    key( const std::string );
    key( const char * );
    std::string getKey() const;
    uint32_t length() const;
};

} // namespace ffx

#endif /* _LIBFTE_SRC_FTE_KEY_H */