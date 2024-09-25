#include "GParsing/HTTP/HTTPResponseException.hpp"
#include <exception>

namespace GParsing {

HTTPResponseException::HTTPResponseException() : std::exception() {}

HTTPResponseException::~HTTPResponseException() {}
} // namespace GParsing
