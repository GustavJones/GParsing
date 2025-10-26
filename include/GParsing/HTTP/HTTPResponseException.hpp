#pragma once
#include <exception>
#include "GParsing/Core/API.hpp"

namespace GParsing {
GPARSING_API class HTTPResponseException : public std::exception {
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
