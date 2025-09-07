#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/HTTP/HTTPMethod.hpp"
#include <string>
#include <vector>

namespace GParsing {
class GPARSING_API HTTPRequest {
public:
  HTTPMethod method = GParsing::HTTPMethod::GPARSING_UNKNOWN;
  std::string uri;
  std::string version;
  std::vector<std::pair<std::string, std::vector<std::string>>> headers;
  std::vector<unsigned char> message;

  HTTPRequest();
  HTTPRequest(HTTPRequest &&) = default;
  HTTPRequest(const HTTPRequest &) = default;
  HTTPRequest &operator=(HTTPRequest &&) = default;
  HTTPRequest &operator=(const HTTPRequest &) = default;
  ~HTTPRequest();

  void ParseRequest(const std::vector<unsigned char> &_request);

  std::vector<unsigned char> CreateRequest() const;

private:
  void _ParseHeaders(const std::string &_headersStr);

  std::pair<std::string, std::vector<std::string>> _ParseHeaderLine(const std::string &_headerLine);

  std::vector<unsigned char> & ConvertToCRLF(std::vector<unsigned char> &_request, const int &_start = 0, const int &_end = -1);
};
} // namespace GParsing

GPARSING_API std::ostream &operator<<(std::ostream &os,
                                      const GParsing::HTTPRequest &obj);
