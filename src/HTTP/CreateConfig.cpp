#include "GParsing/HTTP/CreateConfig.hpp"

namespace GParsing::HTTP {
CreateConfig::CreateConfig() {}
CreateConfig::~CreateConfig() {}

void CreateConfig::CreateRequest(
    GParsing::HTTP::RequestType reqType, std::string reqPath,
    float reqHTTPVersion,
    std::vector<std::pair<std::string, std::string>> reqHeaders,
    char *reqMessage, int reqMessageLength) {
  m_type = GParsing::HTTP::MessageType::Request;

  m_reqType = reqType;
  m_reqPath = reqPath;
  m_reqHTTPVersion = reqHTTPVersion;
  m_reqHeaderList = reqHeaders;
  m_reqMessage = reqMessage;
  m_reqMessageLen = reqMessageLength;
}

void CreateConfig::CreateResponse(
    float respHTTPVersion, int respCode, std::string respCodeMessage,
    std::vector<std::pair<std::string, std::string>> respHeaders,
    char *respMessage, int respMessageLength) {
  m_type = GParsing::HTTP::MessageType::Response;

  m_respHTTPVersion = respHTTPVersion;
  m_respCode = respCode;
  m_respCodeMsg = respCodeMessage;
  m_respHeaderList = respHeaders;
  m_respMessage = respMessage;
  m_respMessageLen = respMessageLength;
}

} // namespace GParsing::HTTP
