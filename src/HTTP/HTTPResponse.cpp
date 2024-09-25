#include "GParsing/HTTP/HTTPResponse.hpp"
#include "GParsing/HTTP/HTTPResponseException.hpp"
#include <iostream>
#include <string>

namespace GParsing {
HTTPResponse::HTTPResponse() {}
HTTPResponse::~HTTPResponse() {}

void HTTPResponse::ParseResponse(const std::vector<unsigned char> &_response) {
  int initLineEndIndex = -1;
  int responseMessageStartIndex = -1;
  int versionStartIndex = -1;
  int versionEndIndex = -1;
  int responseCodeStartIndex = -1;
  int responseCodeEndIndex = -1;
  int responseCodeMessageStartIndex = -1;
  int responseCodeMessageEndIndex = -1;
  int headersStartIndex = -1;
  int headersEndIndex = -1;

  std::string headersStr;
  std::string responseCodeStr;
  std::vector<unsigned char> responseInfo;
  std::vector<unsigned char> responseMessage;

  for (int i = 0; i < _response.size(); i++) {
    if (i > 2) {
      if (_response[i] == '\n' && _response[i - 1] == '\r' &&
          _response[i - 2] == '\n' && _response[i - 3] == '\r') {
        responseMessageStartIndex = i + 1;
        break;
      }
    }

    if (i > 0) {
      if (_response[i] == '\n' && _response[i - 1] == '\n') {
        responseMessageStartIndex = i + 1;
        break;
      }
    }
  }

  for (int i = responseMessageStartIndex; i < _response.size(); i++) {
    responseMessage.push_back(_response[i]);
  }

  for (int i = 0; i < responseMessageStartIndex; i++) {
    responseInfo.push_back(_response[i]);
  }

  ConvertToCRLF(responseInfo);

  for (int i = 1; i < responseInfo.size(); i++) {
    if (responseInfo[i - 1] == '\r' && responseInfo[i] == '\n') {
      initLineEndIndex = i - 1;
      break;
    }
  }

  // Check if response has an initial line
  if (initLineEndIndex < 0) {
    throw GParsing::HTTPResponseException();
    return;
  }

  // Get version start index
  for (int i = 0; i < initLineEndIndex; i++) {
    if (responseInfo[i] == ' ' || responseInfo[i] == 9) {
      continue;
    } else {
      versionStartIndex = i;
      break;
    }
  }

  if (versionStartIndex < 0) {
    throw GParsing::HTTPResponseException();
  }

  // Get version end index
  for (int i = versionStartIndex; i < initLineEndIndex; i++) {
    if (responseInfo[i] != ' ' && responseInfo[i] != 9) {
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
    version += responseInfo[i];
  }

  // Get response code start index
  for (int i = versionEndIndex; i < initLineEndIndex; i++) {
    if (responseInfo[i] == ' ' || responseInfo[i] == 9) {
      continue;
    } else {
      responseCodeStartIndex = i;
      break;
    }
  }

  // Get response code end index
  for (int i = responseCodeStartIndex; i < initLineEndIndex; i++) {
    if (responseInfo[i] != ' ' && responseInfo[i] != 9) {
      continue;
    } else {
      responseCodeEndIndex = i;
      break;
    }
  }

  // Parse response code
  for (int i = responseCodeStartIndex; i < responseCodeEndIndex; i++) {
    responseCodeStr += responseInfo[i];
  }

  try {
    response_code = std::stoi(responseCodeStr);
  } catch (const std::exception &) {
    throw GParsing::HTTPResponseException();
  }

  // Get response code message start index
  for (int i = responseCodeEndIndex; i < initLineEndIndex; i++) {
    if (responseInfo[i] == ' ' || responseInfo[i] == 9) {
      continue;
    } else {
      responseCodeMessageStartIndex = i;
      break;
    }
  }

  // Get response code message end index
  for (int i = responseCodeMessageStartIndex; i < initLineEndIndex; i++) {
    if (responseInfo[i] != ' ' && responseInfo[i] != 9) {
      continue;
    } else {
      responseCodeMessageEndIndex = i;
      break;
    }
  }

  if (responseCodeMessageEndIndex < 0) {
    responseCodeMessageEndIndex = initLineEndIndex;
  }

  // Parse response code message
  for (int i = responseCodeMessageStartIndex; i < responseCodeMessageEndIndex;
       i++) {
    response_code_message += responseInfo[i];
  }

  // Get headers start index
  for (int i = initLineEndIndex + 2; i < responseInfo.size(); i++) {
    if (responseInfo[i] == ' ' || responseInfo[i] == 9) {
      continue;
    } else {
      headersStartIndex = i;
      break;
    }
  }

  // Get headers end index
  headersEndIndex = responseInfo.size() - 4;

  // Parse headers string
  for (int i = headersStartIndex; i < headersEndIndex; i++) {
    headersStr += responseInfo[i];
  }

  _ParseHeaders(headersStr);

  message = responseMessage;
}

std::vector<unsigned char> HTTPResponse::CreateResponse() const {
  std::vector<unsigned char> output;
  std::string responseInfo;

  responseInfo += version;
  responseInfo += ' ';
  responseInfo += std::to_string(response_code);
  responseInfo += ' ';
  responseInfo += response_code_message;
  responseInfo += "\r\n";

  for (const auto &header : headers) {
    responseInfo += header.first + ": ";
    responseInfo += header.second[0];

    for (int i = 1; i < header.second.size(); i++) {
      responseInfo += ", " + header.second[i];
    }

    responseInfo += "\r\n";
  }

  responseInfo += "\r\n";

  for (int i = 0; i < responseInfo.length(); i++) {
    output.push_back(responseInfo[i]);
  }

  for (int i = 0; i < message.size(); i++) {
    output.push_back(message[i]);
  }
  return output;
}

std::pair<std::string, std::vector<std::string>>
HTTPResponse::_ParseHeaderLine(const std::string &_headerLine) {
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

void HTTPResponse::_ParseHeaders(const std::string &_headersStr) {
  const std::string LINE_SEPERATOR = "\r\n";

  int i = 0;
  int headerLineEndIndex = -1;
  int headerLineSeperatorIndex = -1;
  std::string headerLine;

  while (i < _headersStr.length()) {
    headerLineEndIndex = _headersStr.find(LINE_SEPERATOR, i);

    if (headerLineEndIndex < 0) {
      headerLineEndIndex = _headersStr.length();
    }

    while (_headersStr[headerLineEndIndex + LINE_SEPERATOR.length()] == ' ' ||
           _headersStr[headerLineEndIndex + LINE_SEPERATOR.length()] == 9) {
      headerLineEndIndex = _headersStr.find(
          LINE_SEPERATOR, headerLineEndIndex + LINE_SEPERATOR.length());
    }

    headerLine = _headersStr.substr(i, headerLineEndIndex - i);

    headerLineSeperatorIndex = headerLine.find(LINE_SEPERATOR);
    while (!(headerLineSeperatorIndex < 0)) {
      headerLine = headerLine.replace(headerLineSeperatorIndex,
                                      LINE_SEPERATOR.length(), "");
      headerLineSeperatorIndex = headerLine.find(LINE_SEPERATOR);
    }

    headers.push_back(_ParseHeaderLine(headerLine));
    i = headerLineEndIndex + LINE_SEPERATOR.length();
  }
}

std::vector<unsigned char> &
HTTPResponse::ConvertToCRLF(std::vector<unsigned char> &_response,
                            const int &_start, const int &_end) {
  if (_end >= 0) {
    for (int i = _start; i < _end; i++) {
      if (i > 0) {
        if (_response[i] == '\n' && _response[i - 1] != '\r') {
          _response.insert(_response.begin() + i, '\r');
        }
      } else if (i == 0 && _response[i] == '\n') {
        _response.insert(_response.begin() + i, '\r');
      }
    }
  } else {
    for (int i = _start; i < _response.size(); i++) {
      if (i > 0) {
        if (_response[i] == '\n' && _response[i - 1] != '\r') {
          _response.insert(_response.begin() + i, '\r');
        }
      } else if (i == 0 && _response[i] == '\n') {
        _response.insert(_response.begin() + i, '\r');
      }
    }
  }

  return _response;
}
} // namespace GParsing

std::ostream &operator<<(std::ostream &os, const GParsing::HTTPResponse &obj) {
  std::vector<unsigned char> string = obj.CreateResponse();

  for (const auto &c : string) {
    os << c;
  }

  return os;
}
