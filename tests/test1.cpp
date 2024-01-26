#include "GParsing/HTTP/Parse.hpp"
#include <string>

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
  return 0;
}
