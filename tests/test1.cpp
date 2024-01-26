#include "GParsing/HTTP/Create.hpp"
#include "GParsing/HTTP/Parse.hpp"
#include <iostream>
#include <string>
#include <utility>

int main(int argc, char *argv[]) {
  std::string buffer = "GET /path/to/file/index.html HTTP/1.0\n";
  buffer += "header1: value1\n";
  buffer += "header2: value2\n";
  buffer += "header3: value3,\n";
  buffer += " value4, value5\n";
  buffer += " value6, value7\n";
  buffer += "header4: value8, value9\n";
  buffer += "header5: value10,\n";
  buffer += " value11, value12\n";
  buffer += "\n";
  buffer += "This is a test message";

  GParsing::HTTP::Parse http;
  http.ParseHTTP(buffer.c_str(), buffer.length());

  std::vector<std::pair<std::string, std::string>> headers;
  std::pair<std::string, std::string> header1;
  std::pair<std::string, std::string> header2;
  header1.first = "header1";
  header2.first = "header2";
  header1.second = "value1";
  header2.second = "value2";
  headers.push_back(header1);
  headers.push_back(header2);

  std::string message = "this is the message\nHello World";

  GParsing::HTTP::CreateConfig createConfig;
  createConfig.CreateRequest(GParsing::HTTP::RequestType::GET, "/", 1.0,
                             headers, (char *)message.c_str(),
                             message.length());

  GParsing::HTTP::Create create;
  char *output;
  int outputLen;
  output = create.CreateHTTP(createConfig, outputLen);

  std::cout << output << "\n\n\n";
  return 0;
}
