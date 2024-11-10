#include "GParsing/HTTP/HTTPRequest.hpp"
#include "GParsing/HTTP/HTTPMethod.hpp"
#include "GParsing/HTTP/HTTPRequestException.hpp"
#include <iostream>
#include <ostream>
#include <vector>

namespace GParsing {

HTTPRequest::HTTPRequest() {}

HTTPRequest::~HTTPRequest() {}

/// Convert all LF to CRLF in the provided string
std::vector<unsigned char> &
HTTPRequest::ConvertToCRLF(std::vector<unsigned char> &_request,
                           const int &_start, const int &_end) {
  if (_end >= 0) {
    for (int i = _start; i < _end; i++) {
      if (i > 0) {
        if (_request[i] == '\n' && _request[i - 1] != '\r') {
          _request.insert(_request.begin() + i, '\r');
        }
      } else if (i == 0 && _request[i] == '\n') {
        _request.insert(_request.begin() + i, '\r');
      }
    }
  } else {
    for (int i = _start; i < _request.size(); i++) {
      if (i > 0) {
        if (_request[i] == '\n' && _request[i - 1] != '\r') {
          _request.insert(_request.begin() + i, '\r');
        }
      } else if (i == 0 && _request[i] == '\n') {
        _request.insert(_request.begin() + i, '\r');
      }
    }
  }

  return _request;
}

void HTTPRequest::ParseRequest(const std::vector<unsigned char> &_request) {
  int initLineEndIndex = -1;
  int requestMessageStartIndex = -1;
  int methodStartIndex = -1;
  int methodEndIndex = -1;
  int uriStartIndex = -1;
  int uriEndIndex = -1;
  int versionStartIndex = -1;
  int versionEndIndex = -1;
  int headersStartIndex = -1;
  int headersEndIndex = -1;

  std::string headersStr;
  std::string methodStr;
  std::vector<unsigned char> requestInfo;
  std::vector<unsigned char> requestMessage;

  for (int i = 0; i < _request.size(); i++) {
    if (i > 2) {
      if (_request[i] == '\n' && _request[i - 1] == '\r' &&
          _request[i - 2] == '\n' && _request[i - 3] == '\r') {
        requestMessageStartIndex = i + 1;
        break;
      }
    }

    if (i > 0) {
      if (_request[i] == '\n' && _request[i - 1] == '\n') {
        requestMessageStartIndex = i + 1;
        break;
      }
    }
  }

  for (int i = requestMessageStartIndex; i < _request.size(); i++) {
    requestMessage.push_back(_request[i]);
  }

  for (int i = 0; i < requestMessageStartIndex; i++) {
    requestInfo.push_back(_request[i]);
  }

  ConvertToCRLF(requestInfo);

  for (int i = 1; i < requestInfo.size(); i++) {
    if (requestInfo[i - 1] == '\r' && requestInfo[i] == '\n') {
      initLineEndIndex = i - 1;
      break;
    }
  }

  // Check if request has an initial line
  if (initLineEndIndex < 0) {
    throw GParsing::HTTPRequestException();
    return;
  }

  // Get index of http method declaration
  for (int i = 0; i < initLineEndIndex; i++) {
    if (requestInfo[i] == ' ' || requestInfo[i] == 9) {
      continue;
    } else {
      methodStartIndex = i;
      break;
    }
  }

  if (methodStartIndex < 0) {
    throw GParsing::HTTPRequestException();
  }

  // Get index of http method declaration end
  for (int i = methodStartIndex; i < initLineEndIndex; i++) {
    if (requestInfo[i] != ' ' && requestInfo[i] != 9) {
      continue;
    } else {
      methodEndIndex = i;
      break;
    }
  }

  // Parse method
  for (int i = methodStartIndex; i < methodEndIndex; i++) {
    methodStr += requestInfo[i];
  }

  if (methodStr == "GET") {
    method = GParsing::HTTPMethod::GET;
  } else if (methodStr == "HEAD") {
    method = GParsing::HTTPMethod::HEAD;
  } else if (methodStr == "POST") {
    method = GParsing::HTTPMethod::POST;
  } else if (methodStr == "PUT") {
    method = GParsing::HTTPMethod::PUT;
  } else if (methodStr == "DELETE") {
    method = GParsing::HTTPMethod::DELETE;
  } else if (methodStr == "OPTIONS") {
    method = GParsing::HTTPMethod::OPTIONS;
  } else if (methodStr == "TRACE") {
    method = GParsing::HTTPMethod::TRACE;
  }

  // Get URI start index
  for (int i = methodEndIndex; i < initLineEndIndex; i++) {
    if (requestInfo[i] == ' ' || requestInfo[i] == 9) {
      continue;
    } else {
      uriStartIndex = i;
      break;
    }
  }

  if (uriStartIndex < 0) {
    throw GParsing::HTTPRequestException();
  }

  // Get URI end index
  for (int i = uriStartIndex; i < initLineEndIndex; i++) {
    if (requestInfo[i] != ' ' && requestInfo[i] != 9) {
      continue;
    } else {
      uriEndIndex = i;
      break;
    }
  }

  if (uriEndIndex < 0) {
    throw GParsing::HTTPRequestException();
  }

  // Parse URI
  for (int i = uriStartIndex; i < uriEndIndex; i++) {
    uri += requestInfo[i];
  }

  // Get version start index
  for (int i = uriEndIndex; i < initLineEndIndex; i++) {
    if (requestInfo[i] == ' ' || requestInfo[i] == 9) {
      continue;
    } else {
      versionStartIndex = i;
      break;
    }
  }

  if (versionStartIndex < 0) {
    throw GParsing::HTTPRequestException();
  }

  // Get version end index
  for (int i = versionStartIndex; i < initLineEndIndex; i++) {
    if (requestInfo[i] != ' ' && requestInfo[i] != 9) {
      continue;
    } else {
      versionEndIndex = i;
      break;
    }
  }

  if (versionEndIndex < 0) {
    versionEndIndex = initLineEndIndex;
  }

  // Parse Version
  for (int i = versionStartIndex; i < versionEndIndex; i++) {
    version += requestInfo[i];
  }

  // Get headers start index
  for (int i = initLineEndIndex + 2; i < requestInfo.size(); i++) {
    if (requestInfo[i] == ' ' || requestInfo[i] == 9) {
      continue;
    } else {
      headersStartIndex = i;
      break;
    }
  }

  // Get headers end index
  headersEndIndex = requestInfo.size() - 4;

  // Parse headers string
  for (int i = headersStartIndex; i < headersEndIndex; i++) {
    headersStr += requestInfo[i];
  }

  _ParseHeaders(headersStr);

  message = requestMessage;
}

std::vector<unsigned char> HTTPRequest::CreateRequest() const {
  std::vector<unsigned char> output;
  std::string requestInfo;
  std::string methodStr;

  switch (method) {
  case GParsing::HTTPMethod::GET:
    methodStr = "GET";
    break;
  case GParsing::HTTPMethod::HEAD:
    methodStr = "HEAD";
    break;
  case GParsing::HTTPMethod::POST:
    methodStr = "POST";
    break;
  case GParsing::HTTPMethod::PUT:
    methodStr = "PUT";
    break;
  case GParsing::HTTPMethod::TRACE:
    methodStr = "TRACE";
    break;
  case GParsing::HTTPMethod::DELETE:
    methodStr = "DELETE";
    break;
  case GParsing::HTTPMethod::OPTIONS:
    methodStr = "OPTIONS";
    break;
  default:
    throw GParsing::HTTPRequestException();
  }

  requestInfo += methodStr;

  requestInfo += ' ';
  requestInfo += uri;
  requestInfo += ' ';
  requestInfo += version;
  requestInfo += "\r\n";

  for (const auto &header : headers) {
    requestInfo += header.first + ": ";
    requestInfo += header.second[0];

    for (int i = 1; i < header.second.size(); i++) {
      requestInfo += ", " + header.second[i];
    }

    requestInfo += "\r\n";
  }

  requestInfo += "\r\n";

  for (int i = 0; i < requestInfo.length(); i++) {
    output.push_back(requestInfo[i]);
  }

  for (int i = 0; i < message.size(); i++) {
    output.push_back(message[i]);
  }
  return output;
}

std::pair<std::string, std::vector<std::string>>
HTTPRequest::_ParseHeaderLine(const std::string &_headerLine) {
  const char VALUE_SEPERATOR = ',';

  int i = 0;
  int headerKeyStartIndex = -1;
  int headerKeyEndIndex = -1;

  int valueStartIndex = -1;
  int valueEndIndex = -1;

  std::string headerKey;
  std::vector<std::string> headerValues;
  std::pair<std::string, std::vector<std::string>> output;

  // Key parsing
  for (int j = 0; j < _headerLine.length(); j++) {
    if (_headerLine[j] == ' ' || _headerLine[j] == 9) {
      continue;
    } else {
      headerKeyStartIndex = j;
      break;
    }
  }

  for (int j = 0; j < _headerLine.length(); j++) {
    if (_headerLine[j] != ' ' && _headerLine[j] != 9 && _headerLine[j] != ':') {
      continue;
    } else {
      headerKeyEndIndex = j;
      break;
    }
  }

  headerKey = _headerLine.substr(headerKeyStartIndex,
                                 headerKeyEndIndex - headerKeyStartIndex);

  // Value Parsing
  i = headerKeyEndIndex;
  while (i < _headerLine.length()) {
    valueStartIndex = -1;
    valueEndIndex = -1;

    for (int j = i; j < _headerLine.length(); j++) {
      if (_headerLine[j] == ' ' || _headerLine[j] == 9 ||
          _headerLine[j] == ':' || _headerLine[j] == VALUE_SEPERATOR) {
        continue;
      } else {
        valueStartIndex = j;
        break;
      }
    }

    for (int j = valueStartIndex; j < _headerLine.length(); j++) {
      if (_headerLine[j] != ' ' && _headerLine[j] != 9 &&
          _headerLine[j] != VALUE_SEPERATOR) {
        continue;
      } else {
        valueEndIndex = j;
        break;
      }
    }

    if (valueEndIndex < 0) {
      valueEndIndex = _headerLine.length();
    }

    headerValues.push_back(
        _headerLine.substr(valueStartIndex, valueEndIndex - valueStartIndex));
    i = valueEndIndex;
  }

  output.first = headerKey;
  output.second = headerValues;
  return output;
}

void HTTPRequest::_ParseHeaders(const std::string &_headersStr) {
  const std::string LINE_SEPERATOR = "\r\n";

  int i = 0;
  int headerLineEndIndex = -1;
  int headerLineSeperatorIndex = -1;
  std::string headerLine;

  while (i < _headersStr.length()) {
    headerLineEndIndex = _headersStr.find(LINE_SEPERATOR, i);

    if (headerLineEndIndex == _headersStr.npos) {
      headerLineEndIndex = _headersStr.length();
    } else {
      while (_headersStr[headerLineEndIndex + LINE_SEPERATOR.length()] == ' ' ||
             _headersStr[headerLineEndIndex + LINE_SEPERATOR.length()] == 9) {
        headerLineEndIndex = _headersStr.find(
            LINE_SEPERATOR, headerLineEndIndex + LINE_SEPERATOR.length());
      }
    }

    headerLine = _headersStr.substr(i, headerLineEndIndex - i);

    headerLineSeperatorIndex = headerLine.find(LINE_SEPERATOR);
    while (headerLineSeperatorIndex != headerLine.npos) {
      headerLine = headerLine.replace(headerLineSeperatorIndex,
                                      LINE_SEPERATOR.length(), "");
      headerLineSeperatorIndex = headerLine.find(LINE_SEPERATOR);
    }

    headers.push_back(_ParseHeaderLine(headerLine));
    i = headerLineEndIndex + LINE_SEPERATOR.length();
  }
}
} // namespace GParsing

std::ostream &operator<<(std::ostream &os, const GParsing::HTTPRequest &obj) {
  std::vector<unsigned char> string = obj.CreateRequest();

  for (const auto &c : string) {
    os << c;
  }

  return os;
}
