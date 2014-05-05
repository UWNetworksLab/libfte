#include <assert.h>

#include "key.h"

namespace ffx {

key::key( const std::string key ) {
    _key = key;
}

key::key( const char * key ) {
    _key = key;
}

std::string key::getKey() const {
    return _key;
}

uint32_t key::length() const {
    return _key.length();
}

}