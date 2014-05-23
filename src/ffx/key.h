#ifndef _FFX_KEY_H
#define _FFX_KEY_H

#include <stdint.h>

#include <string>

#include <gmpxx.h>

namespace ffx {

const uint32_t kFFXKeyLengthInBytes = 16;
const uint32_t kFFXKeyLengthInNibbles = kFFXKeyLengthInBytes * 2;
const uint32_t kFFXKeyLengthInBits = kFFXKeyLengthInBytes * 8;

/*
 * This is a wrapper class for a hex-specified key for FTE/FFX.
 */
class Key {
 private:
  std::string key_;

 public:
  Key() {};
  Key(const std::string);
  Key(const char *);

  std::string get_key() const;
  uint32_t length() const;
};

} // namespace ffx

#endif /* _FFX_KEY_H */
