#pragma once
#include <vector>

#include "GParsing/HTTP/HTTPRequest.hpp"
#include "GParsing/HTTP/HTTPResponse.hpp"

namespace GParsing {
inline std::vector<unsigned char> ConvertToCharArray(const char *_c,
                                                     const int &_len) {
  std::vector<unsigned char> output;
  for (int i = 0; i < _len; i++) {
    output.push_back(_c[i]);
  }
  return output;
}

inline void ConvertToCharPointer(std::vector<unsigned char> _c, char *_out) {
  for (int i = 0; i < _c.size(); i++) {
    _out[i] = _c[i];
  }
}
} // namespace GParsing
