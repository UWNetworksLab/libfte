#include <assert.h>

#include "key.h"

namespace fte {

key::key( const std::string key ) 
{
    _key = key;
    
    assert(_key.length()==32);
}

key::key( const char * key ) 
{
    _key = key;
    
    assert(_key.length()==32);
}

std::string key::getKey() const {
    return _key;
}

// bool validateKey( )

uint32_t key::length() const {
    return 16;
}

}