#include "GParsing/GParsing.hpp"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  const std::string buffer =
      "<!-- This is a comment -->  <!DOCTYPE html SYSTEM 'about:legacy-compat'>\n< html >\n<head>\n<title> Page Title</ title>\n</ head>\n<body>\n"
      "<a href = \'https://www.geeksforgeeks.org/\' target = \"_blank\">"
      "Test < h1 > Hello? </ h1>GeeksforGeeks</ a>\n<br arg = \"Value\"/>\n<script></script></ body>\n< / html > ";

  // const std::string buffer =
  //     "<html>\n<head>\n<title> Page Title</ title>\n</ head>\n<body>\n"
  //     "<a href = \"https://www.geeksforgeeks.org/\" target = \"_blank\">"
  //     "GeeksforGeeks</ a>\n</ body>\n< / html > ";

  //const std::string buffer =
  //  "<!-- This is a comment -->  <!DOCTYPE html SYSTEM 'about:legacy-compat'>\n< html >\n<head>\n</ head>\n<body>\n"
  //  "<br arg = \"Value\"/>\n</ body>\n< / html > ";
  
  GParsing::HTMLDocument<char> doc;
  doc.Parse(buffer.c_str(), buffer.length());

  auto root = doc.GetRootElement();

  const std::string indentCharacters = "  ";
  std::cout << doc.Serialize(indentCharacters.c_str(), indentCharacters.length()).data() << std::endl;

  std::cin.get();
  return 0;
}
