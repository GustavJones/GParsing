#pragma once
#include "GParsing/HTTP/Enums.hpp"
#include "GParsing/LibraryAPI.hpp"
#include <string>
#include <utility>
#include <vector>

namespace GParsing {

namespace HTTP {

class GPARSING_API CreateConfig {
public:
  GParsing::HTTP::MessageType m_type;

  RequestType m_reqType;
  std::string m_reqPath;
  float m_reqHTTPVersion;
  std::vector<std::pair<std::string, std::string>> m_reqHeaderList;
  char *m_reqMessage;
  int m_reqMessageLen;

  float m_respHTTPVersion;
  std::vector<std::pair<std::string, std::string>> m_respHeaderList;
  int m_respCode;
  std::string m_respCodeMsg;
  char *m_respMessage;
  int m_respMessageLen;

public:
  CreateConfig();
  CreateConfig(CreateConfig &&) = default;
  CreateConfig(const CreateConfig &) = default;
  CreateConfig &operator=(CreateConfig &&) = default;
  CreateConfig &operator=(const CreateConfig &) = default;
  ~CreateConfig();

  void
  CreateRequest(GParsing::HTTP::RequestType reqType, std::string reqPath,
                float reqHTTPVersion,
                std::vector<std::pair<std::string, std::string>> reqHeaders,
                char *reqMessage, int reqMessageLength);
  void
  CreateResponse(float respHTTPVersion, int respCode,
                 std::string respCodeMessage,
                 std::vector<std::pair<std::string, std::string>> respHeaders,
                 char *respMessage, int respMessageLength);

private:
};
} // namespace HTTP

} // namespace GParsing
