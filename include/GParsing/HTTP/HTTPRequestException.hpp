#pragma once
#include <exception>

namespace GParsing {
class HTTPRequestException : public std::exception {
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
