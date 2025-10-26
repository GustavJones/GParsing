#pragma once
#include <exception>
#include "GParsing/Core/API.hpp"

namespace GParsing {
GPARSING_API class HTTPRequestException : public std::exception {
public:
  HTTPRequestException();
  HTTPRequestException(HTTPRequestException &&) = default;
  HTTPRequestException(const HTTPRequestException &) = default;
  HTTPRequestException &operator=(HTTPRequestException &&) = default;
  HTTPRequestException &operator=(const HTTPRequestException &) = default;
  ~HTTPRequestException();

private:
};
} // namespace GParsing
