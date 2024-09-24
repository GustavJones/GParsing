#include "GParsing/HTTP/HTTPRequestException.hpp"
#include <exception>

namespace GParsing {

HTTPRequestException::HTTPRequestException() : std::exception() {}

HTTPRequestException::~HTTPRequestException() {}
} // namespace GParsing
