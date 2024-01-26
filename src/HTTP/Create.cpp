#include "GParsing/HTTP/Create.hpp"
#include "GParsing/HTTP/Enums.hpp"
#include <string>

namespace GParsing::HTTP {
Create::Create() { m_output = new char[1](); };
Create::~Create() { delete[] m_output; };
char *Create::CreateHTTP(const CreateConfig &createConfig,
                         int &bufferOutputLen) {
  m_msgType = createConfig.m_type;
  std::string output;
  delete[] m_output;

  if (m_msgType == GParsing::HTTP::MessageType::Request) {
    m_reqType = createConfig.m_reqType;
    m_reqPath = createConfig.m_reqPath;
    m_reqHTTPVersion = createConfig.m_reqHTTPVersion;
    m_reqHeaderList = createConfig.m_reqHeaderList;
    m_reqMessage = createConfig.m_reqMessage;
    m_reqMessageLen = createConfig.m_reqMessageLen;

    // Request Type
    switch (m_reqType) {
    case GET:
      output += "GET ";
      break;
    case POST:
      output += "POST ";
      break;
    case HEAD:
      output += "HEAD ";
      break;
    case UNKOWN:
      return nullptr;
      break;
    }

    // Path
    output += m_reqPath + ' ';

    // Version
    std::string version = std::to_string(m_reqHTTPVersion);
    if (version.length() < 3) {
      version += ".0";
    } else if (version.length() > 3) {
      version.erase(3, version.length() - 3);
    }

    output += "HTTP/" + version + "\r\n";

    // Headers
    for (auto headers : m_reqHeaderList) {
      output += headers.first + ": " + headers.second + "\r\n";
    }

    // Open line before message
    output += "\r\n";

    // Message
    bufferOutputLen = output.length() + m_reqMessageLen + 1;
    m_output = new char[output.length() + m_reqMessageLen + 1]();
    for (int i = 0; i < output.length(); i++) {
      m_output[i] = output[i];
    }

    for (int i = output.length(); i < output.length() + m_reqMessageLen; i++) {
      m_output[i] = m_reqMessage[i - output.length()];
    }

    m_output[bufferOutputLen - 1] = 0;

    return m_output;

  } else if (m_msgType == GParsing::HTTP::MessageType::Response) {
    m_respHTTPVersion = createConfig.m_respHTTPVersion;
    m_respHeaderList = createConfig.m_respHeaderList;
    m_respCode = createConfig.m_respCode;
    m_respCodeMsg = createConfig.m_respCodeMsg;
    m_respMessage = createConfig.m_respMessage;
    m_respMessageLen = createConfig.m_respMessageLen;

    // Version
    std::string version = std::to_string(m_respHTTPVersion);
    if (version.length() < 3) {
      version += ".0";
    } else if (version.length() > 3) {
      version.erase(3, version.length() - 3);
    }
    output += "HTTP/" + version + ' ';

    // Status Code
    output += std::to_string(m_respCode) + ' ';

    // Status Code Message
    output += m_respCodeMsg + "\r\n";

    // Headers
    for (auto headers : m_respHeaderList) {
      output += headers.first + ": " + headers.second + "\r\n";
    }

    // Open line before message
    output += "\r\n";

    // Message
    bufferOutputLen = output.length() + m_respMessageLen + 1;
    m_output = new char[output.length() + m_respMessageLen + 1]();
    for (int i = 0; i < output.length(); i++) {
      m_output[i] = output[i];
    }

    for (int i = output.length(); i < output.length() + m_respMessageLen; i++) {
      m_output[i] = m_respMessage[i - output.length()];
    }

    m_output[bufferOutputLen - 1] = 0;

    return m_output;
  }

  return 0;
}
} // namespace GParsing::HTTP
