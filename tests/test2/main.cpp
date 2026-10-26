#include "GParsing/GParsing.hpp"
#include <string>

int main(int argc, char *argv[]) {
  const std::string test_response_string =
      "HTTP/1.0 200 OK\nDate: Fri, 31 Dec 1999 23:59:59 GMT\nContent-Type: "
      "text/html\nContent-Length: 1354\n\n<html>\n<body>\n<h1>Happy New "
      "Millennium!</h1>\n(more file contents)\n  .\n  .\n  .\n</body>\n</html>";

	GParsing::HTTPString<unsigned char> test_response = test_response_string;
	GParsing::HTTPString<unsigned char> output;

  std::cout << "TEST2" << std::endl << std::endl;
	std::cout << "Input: \n" << test_response << std::endl;
	
	GParsing::HTTPResponse<unsigned char> resp;
	resp.Parse(test_response);
	resp.Serialize(output);

	std::cout << "Output: \n" << output << std::endl;

  return 0;
}
