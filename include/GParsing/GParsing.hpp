#pragma once
#include <vector>

#include "GParsing/HTTP/GParsing-HTTP.hpp"
#include "GParsing/HTML/GParsing-HTML.hpp"

namespace GParsing {
inline std::vector<unsigned char> ConvertToCharArray(const char *_c, const int &_len) {
  std::vector<unsigned char> output;
  output.reserve(_len);

  for (int i = 0; i < _len; i++) {
    output.push_back(_c[i]);
  }
  return output;
}

inline const unsigned char *const ConvertToCharPointer(const std::vector<unsigned char> &_c) {
  return _c.data();
}
} // namespace GParsing
