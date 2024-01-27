#pragma once
#include "Enums.hpp"
#include "GParsing/LibraryAPI.hpp"
#include <string>
#include <vector>

namespace GParsing {
namespace HTTP {
class GPARSING_API Parse {
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

public:
  Parse();
  Parse(Parse &&) = delete;
  Parse(const Parse &) = delete;
  Parse &operator=(Parse &&) = delete;
  Parse &operator=(const Parse &) = delete;
  ~Parse();

  int ParseHTTP(const char *buffer, int bufferLen,
                MessageType msgType = MessageType::Request);

  RequestType GetRequestType() { return m_reqType; };
  std::string GetRequestPath() { return m_reqPath; };
  float GetRequestHTTPVersion() { return m_reqHTTPVersion; };
  std::vector<std::pair<std::string, std::string>> GetRequestHeaders() {
    return m_reqHeaderList;
  };
  char *GetRequestMessage() { return m_reqMessage; };
  int GetRequestMessageLength() { return m_reqMessageLen; };

  float GetResponseHTTPVersion() { return m_respHTTPVersion; };
  std::vector<std::pair<std::string, std::string>> GetResponseHeaders() {
    return m_respHeaderList;
  };
  int GetResponseCode() { return m_respCode; };
  std::string GetResponseCodeMessage() { return m_respCodeMsg; };
  char *GetResponseMessage() { return m_respMessage; };
  int GetResponseMessageLength() { return m_respMessageLen; };
};
} // namespace HTTP

} // namespace GParsing
