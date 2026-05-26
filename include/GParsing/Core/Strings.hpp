#pragma once
#include <cmath>
#include <limits>
#include <string>
#include <sstream>

namespace GParsing {
  template<typename number_t>
  std::string to_string(const number_t _num) {
    if (std::isnan(_num)) {
      return "nan";
    }

    if (std::isinf(_num)) {
      return _num < 0 ? "-inf" : "inf";
    }

    std::stringstream ss;
    ss.precision(std::numeric_limits<number_t>::max_digits10);
    ss << _num;

    if (ss.str() == "-0") {
      return "0";
    }

    return ss.str();
  }
}
