#include "GParsing/GParsing.hpp"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  const std::string test_request_string1 =
      "GET /path/file.html HTTP/1.0\nFrom: someuser@jmarshall.com,\n "
      "another@gmail.com\nUser-Agent: HTTPTool/1.0\nDate: Fri, 31 Dec 1999 "
      "23:59:59 GMT\nContent-Type: text/plain\nTransfer-Encoding: "
      "chunked\n\n1a; "
      "ignore-stuff-"
      "here\nabcdefghijklmnopqrstuvwxyz\n10\n1234567890abcdef\n0\n\n";
  const std::string test_request_string2 =
      "GET /path/file.html HTTP/1.0\nFrom: someuser@jmarshall.com,\n "
      "another@gmail.com\nUser-Agent: HTTPTool/1.0\nDate: Fri, 31 Dec 1999 "
      "23:59:59 GMT\nContent-Type: text/plain\n\n1a; "
      "ignore-stuff-"
      "here\nabcdefghijklmnopqrstuvwxyz\n10\n1234567890abcdef\n0\n\n";

  GParsing::HTTPString<unsigned char> test_request = test_request_string1;
  GParsing::HTTPString<unsigned char> output;

  std::cout << "TEST1" << std::endl << std::endl;
	std::cout << "Input: \n" << test_request << std::endl;

  GParsing::HTTPRequest<unsigned char> req;
  req.Parse(test_request);
	req.Serialize(output);

	std::cout << "Output: \n" << output << std::endl;
  return 0;
}
