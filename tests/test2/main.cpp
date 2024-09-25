#include "GParsing/GParsing.hpp"
#include "GParsing/HTTP/HTTPResponse.hpp"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  const std::string test_response_string =
      "HTTP/1.0 200 OK\nDate: Fri, 31 Dec 1999 23:59:59 GMT\nContent-Type: "
      "text/html\nContent-Length: 1354\n\n<html>\n<body>\n<h1>Happy New "
      "Millennium!</h1>\n(more file contents)\n  .\n  .\n  .\n</body>\n</html>";

  std::vector<unsigned char> test_response = GParsing::ConvertToCharArray(
      test_response_string.c_str(), test_response_string.length());

  std::cout << "TEST2" << std::endl << std::endl;

  GParsing::HTTPResponse resp;
  resp.ParseResponse(test_response);

  std::cout << resp << std::endl;

  return 0;
}
