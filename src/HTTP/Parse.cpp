#include "GParsing/HTTP/Parse.hpp"
#include "GParsing/Find.hpp"
#include "GParsing/HTTP/Enums.hpp"
#include <string>
#include <utility>
#include <vector>

namespace GParsing::HTTP {
Parse::Parse()
    : m_reqHTTPVersion(-1), m_reqType(RequestType::UNKOWN), m_reqPath(""),
      m_respHTTPVersion(-1), m_respCode(-1), m_respCodeMsg("") {
  m_reqMessage = new char[1]();
  m_respMessage = new char[1]();
}

Parse::~Parse() {
  delete[] m_reqMessage;
  delete[] m_respMessage;
}

int Parse::ParseHTTP(const char *buffer, int bufferLen, MessageType msgType) {
  m_msgType = msgType;

  // Request Parsing
  if (m_msgType == MessageType::Request) {
    // ------- Initial Line Parsing ------
    int initLineEndIndex = FindString(buffer, bufferLen, "\n");
    int getIndex = FindString(buffer, bufferLen, "GET", 0, initLineEndIndex);
    int postIndex = FindString(buffer, bufferLen, "POST", 0, initLineEndIndex);
    int headIndex = FindString(buffer, bufferLen, "HEAD", 0, initLineEndIndex);

    int pathIndex = FindString(buffer, bufferLen, " /", 0, initLineEndIndex);
    int httpVersionIndex = FindString(buffer, bufferLen, " HTTP/");

    // RequestType Parse
    if (getIndex != -1 && getIndex < pathIndex && getIndex < httpVersionIndex) {
      m_reqType = RequestType::GET;
    } else if (postIndex != -1 && postIndex < pathIndex &&
               postIndex < httpVersionIndex) {
      m_reqType = RequestType::POST;
    } else if (headIndex != -1 && headIndex < pathIndex &&
               headIndex < httpVersionIndex) {
      m_reqType = RequestType::HEAD;
    } else {
      return -1;
    }

    // RequestPath Parse
    int i = pathIndex + 1;
    while (i < httpVersionIndex) {
      m_reqPath += buffer[i];
      i++;
    }

    // RequestHTTPVersion Parse
    i = httpVersionIndex + 6;
    std::string versionStr;

    while (i < initLineEndIndex && buffer[i] != ' ') {
      versionStr += buffer[i];
      i++;
    }
    m_reqHTTPVersion = std::stof(versionStr);

    // -------- Header Parsing --------
    bool headerEnd = false;
    bool prevHeader = false;
    int headerLineStartIndex = initLineEndIndex + 1;
    int headerLineEndIndex;
    int headerSeperatorIndex;
    std::string headerKey;
    std::string headerValue;

    // Loop through header list
    while (!headerEnd) {
      headerKey = "";
      headerValue = "";
      prevHeader = false;

      // Line End Index
      headerLineEndIndex =
          FindString(buffer, bufferLen, "\n", headerLineStartIndex);

      // Seperator Index
      headerSeperatorIndex = FindString(
          buffer, bufferLen, ":", headerLineStartIndex, headerLineEndIndex);

      // Get Header Key
      i = headerLineStartIndex;
      while (i < headerLineEndIndex) {
        if (buffer[i] == ' ' && i == headerLineStartIndex) {
          prevHeader = true;
        }

        if (headerSeperatorIndex != -1 && i == headerSeperatorIndex) {
          break;
        }

        if (prevHeader) {
          headerValue += buffer[i];
        } else {
          headerKey += buffer[i];
        }
        i++;
      }

      // Get Header Value
      if (!prevHeader) {
        i = headerSeperatorIndex + 1;
        while (i < headerLineEndIndex) {
          headerValue += buffer[i];
          i++;
        }
      }

      // Key Cleanup
      if (!prevHeader) {
        i = 0;
        while (headerKey[i] == ' ') {
          headerKey.erase(i, 1);
        }

        i = headerKey.length() - 1;
        while (headerKey[i] == ' ') {
          headerKey.erase(i, 1);
          i--;
        }
      }

      // Value Cleanup
      i = 0;
      while (headerValue[i] == ' ') {
        headerValue.erase(i, 1);
      }

      i = headerValue.length() - 1;
      while (headerValue[i] == ' ' || headerValue[i] == '\r') {
        headerValue.erase(i, 1);
        i--;
      }

      if (!prevHeader) {
        std::pair<std::string, std::string> header;
        header.first = headerKey;
        header.second = headerValue;
        m_reqHeaderList.push_back(header);
      } else {
        m_reqHeaderList[m_reqHeaderList.size() - 1].second += " " + headerValue;
      }

      // Increment and check for break
      headerLineStartIndex = headerLineEndIndex + 1;
      if (buffer[headerLineEndIndex + 1] == '\n') {
        headerEnd = true;
      }
    }

    // ----- Message Parsing --------
    int messageStartIndex = FindString(buffer, bufferLen, "\n\r\n", i) + 3;
    if (messageStartIndex == -1) {
      messageStartIndex = FindString(buffer, bufferLen, "\n\n", i) + 2;
    }

    delete[] m_reqMessage;
    m_reqMessageLen = bufferLen - messageStartIndex;
    m_reqMessage = new char[m_reqMessageLen]();

    i = messageStartIndex;
    while (i < bufferLen) {
      m_reqMessage[i - messageStartIndex] = buffer[i];
      i++;
    }
  }
  // Response Parsing
  else if (m_msgType == MessageType::Response) {
    // -------- Initial Line Parsing -------

    // HTTP Version Parsing
    int i = 0;
    int httpVersionIndex = FindString(buffer, bufferLen, "HTTP/") + 5;
    std::string httpVersionStr;

    i = httpVersionIndex;
    while (buffer[i] != ' ') {
      httpVersionStr += buffer[i];
      i++;
    }

    m_respHTTPVersion = std::stof(httpVersionStr);

    // Status Code Parsing
    std::string statusCodeStr;

    while (buffer[i] == ' ') {
      i++;
    }

    while (buffer[i] != ' ') {
      statusCodeStr += buffer[i];
      i++;
    }

    m_respCode = std::stoi(statusCodeStr);

    // Status Code Message Parsing
    while (buffer[i] == ' ') {
      i++;
    }

    while (buffer[i] != '\n' && buffer[i] != '\r') {
      m_respCodeMsg += buffer[i];
      i++;
    }

    // -------- Header Parsing --------
    bool headerEnd = false;
    bool prevHeader = false;
    int headerLineStartIndex = i + 1;
    int headerLineEndIndex;
    int headerSeperatorIndex;
    std::string headerKey;
    std::string headerValue;

    // Loop through header list
    while (!headerEnd) {
      headerKey = "";
      headerValue = "";
      prevHeader = false;

      // Line End Index
      headerLineEndIndex =
          FindString(buffer, bufferLen, "\n", headerLineStartIndex);

      // Seperator Index
      headerSeperatorIndex = FindString(
          buffer, bufferLen, ":", headerLineStartIndex, headerLineEndIndex);

      // Get Header Key
      i = headerLineStartIndex;
      while (i < headerLineEndIndex) {
        if (buffer[i] == ' ' && i == headerLineStartIndex) {
          prevHeader = true;
        }

        if (headerSeperatorIndex != -1 && i == headerSeperatorIndex) {
          break;
        }

        if (prevHeader) {
          headerValue += buffer[i];
        } else {
          headerKey += buffer[i];
        }
        i++;
      }

      // Get Header Value
      if (!prevHeader) {
        i = headerSeperatorIndex + 1;
        while (i < headerLineEndIndex) {
          headerValue += buffer[i];
          i++;
        }
      }

      // Key Cleanup
      if (!prevHeader) {
        i = 0;
        while (headerKey[i] == ' ') {
          headerKey.erase(i, 1);
        }

        i = headerKey.length() - 1;
        while (headerKey[i] == ' ') {
          headerKey.erase(i, 1);
          i--;
        }
      }

      // Value Cleanup
      i = 0;
      while (headerValue[i] == ' ') {
        headerValue.erase(i, 1);
      }

      i = headerValue.length() - 1;
      while (headerValue[i] == ' ' || headerValue[i] == '\r') {
        headerValue.erase(i, 1);
        i--;
      }

      if (!prevHeader) {
        std::pair<std::string, std::string> header;
        header.first = headerKey;
        header.second = headerValue;
        m_respHeaderList.push_back(header);
      } else {
        m_respHeaderList[m_respHeaderList.size() - 1].second +=
            " " + headerValue;
      }

      // Increment and check for break
      headerLineStartIndex = headerLineEndIndex + 1;
      if (buffer[headerLineEndIndex + 1] == '\n') {
        headerEnd = true;
      }
    }

    // ----- Message Parsing --------
    int messageStartIndex = FindString(buffer, bufferLen, "\n\r\n", i) + 3;
    if (messageStartIndex == -1) {
      messageStartIndex = FindString(buffer, bufferLen, "\n\n", i) + 2;
    }

    delete[] m_respMessage;
    m_respMessageLen = bufferLen - messageStartIndex;
    m_respMessage = new char[m_respMessageLen]();

    i = messageStartIndex;
    while (i < bufferLen) {
      m_respMessage[i - messageStartIndex] = buffer[i];
      i++;
    }
  }

  return 0;
}
} // namespace GParsing::HTTP
