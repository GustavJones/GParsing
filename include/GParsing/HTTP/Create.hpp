#pragma once
#include "Enums.hpp"
#include "GParsing/HTTP/CreateConfig.hpp"
#include "GParsing/LibraryAPI.hpp"

namespace GParsing {
namespace HTTP {
class GPARSING_API Create {
private:
  MessageType m_msgType;

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

  char *m_output;

public:
  Create();
  Create(Create &&) = delete;
  Create(const Create &) = delete;
  Create &operator=(Create &&) = delete;
  Create &operator=(const Create &) = delete;
  ~Create();

  char *CreateHTTP(const CreateConfig &createConfig, int &bufferOutputLen);
};
} // namespace HTTP

} // namespace GParsing
