#pragma once
#include <vector>

namespace GParsing {
inline std::vector<unsigned char> ConvertToCharArray(const char *_c,
                                                     const int &_len) {
  std::vector<unsigned char> output;
  for (int i = 0; i < _len; i++) {
    output.push_back(_c[i]);
  }
  return output;
}
} // namespace GParsing
