#include "GParsing/GParsing.hpp"
#include "GParsing/HTTP/HTTPRequest.hpp"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  const std::string test_request_string =
      "GET /path/file.html HTTP/1.0\nFrom: someuser@jmarshall.com,\n "
      "another@gmail.com\nUser-Agent: HTTPTool/1.0\n\nThis is a test";

  std::vector<unsigned char> test_request = GParsing::ConvertToCharArray(
      test_request_string.c_str(), test_request_string.length());

  std::cout << "TEST1" << std::endl << std::endl;

  GParsing::HTTPRequest req;
  req.ParseRequest(test_request);

  std::cout << req << std::endl;

  return 0;
}
