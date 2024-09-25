#pragma once
#include "GParsing/Core/API.hpp"
#include <string>
#include <vector>

namespace GParsing {
class GPARSING_API HTTPResponse {
public:
  int response_code;
  std::string response_code_message;
  std::string version;
  std::vector<std::pair<std::string, std::vector<std::string>>> headers;
  std::vector<unsigned char> message;

  HTTPResponse();
  HTTPResponse(HTTPResponse &&) = default;
  HTTPResponse(const HTTPResponse &) = default;
  HTTPResponse &operator=(HTTPResponse &&) = default;
  HTTPResponse &operator=(const HTTPResponse &) = default;
  ~HTTPResponse();

  void ParseResponse(const std::vector<unsigned char> &_response);
  std::vector<unsigned char> CreateResponse() const;

private:
  void _ParseHeaders(const std::string &_headersStr);
  std::pair<std::string, std::vector<std::string>>
  _ParseHeaderLine(const std::string &_headerLine);
  std::vector<unsigned char> &
  ConvertToCRLF(std::vector<unsigned char> &_request, const int &_start = 0,
                const int &_end = -1);
};
} // namespace GParsing

GPARSING_API std::ostream &operator<<(std::ostream &os,
                                      const GParsing::HTTPResponse &obj);
