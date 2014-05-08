#include <assert.h>

#include "ffx/key.h"

namespace ffx {

Key::Key(const std::string key) {
  key_ = key;
}

Key::Key(const char * key) {
  key_ = key;
}

std::string Key::get_key() const {
  return key_;
}

uint32_t Key::length() const {
  return key_.length();
}

}
