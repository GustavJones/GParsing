#pragma once
#include <exception>

namespace GParsing {
class HTTPResponseException : public std::exception {
public:
  HTTPResponseException();
  HTTPResponseException(HTTPResponseException &&) = default;
  HTTPResponseException(const HTTPResponseException &) = default;
  HTTPResponseException &operator=(HTTPResponseException &&) = default;
  HTTPResponseException &operator=(const HTTPResponseException &) = default;
  ~HTTPResponseException();

private:
};
} // namespace GParsing
