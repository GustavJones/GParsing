#include "GParsing/GParsing.hpp"
#include "GParsing/Core/Buffers.hpp"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  const char string1[] = "    \"This is a test\"     ";

  GParsing::JSONString<char> s;
  std::cout << s.Parse(string1, sizeof(string1) / sizeof(string1[0])) << std::endl;

  std::vector<char> buffer;
  s.Serialize(buffer);
  buffer.push_back('\0');
  s.Append('\0');
  std::cout << s.GetString().data() << std::endl;
  std::cout << buffer.data() << std::endl;

  std::cout << "--------------" << std::endl;

  const char string2[] = "    true     ";
  GParsing::JSONExpression<char> exp;
  std::cout << exp.Parse(string2, sizeof(string2) / sizeof(string2[0]) - 1) << std::endl;

  exp.Serialize(buffer);
  buffer.push_back('\0');
  std::cout << buffer.data() << std::endl;

  std::cout << "--------------" << std::endl;

  const char string3[] = "   [ 3.23451234, [ true ], [ true ]]     ";
  GParsing::JSONArray<char> a;
  std::cout << a.Parse(string3, sizeof(string3) / sizeof(string3[0]) - 1) << std::endl;

  a.Serialize(buffer);
  buffer.push_back('\0');
  std::cout << buffer.data() << std::endl;

  std::cout << "--------------" << std::endl;

  const char string4[] = "   [true, true, true]     ";
  GParsing::JSONValue<char> val;
  std::cout << val.Parse(string4, sizeof(string4) / sizeof(string4[0]) - 1) << std::endl;

  std::cout << "--------------" << std::endl;

  const char string5[] = " { \"Key1\" : [ { \"Hello\": 32.543 }, null, false], \"Key2\" : \"Value2\"}   ";
  GParsing::JSONObject<char> obj;
  std::cout << obj.Parse(string5, sizeof(string5) / sizeof(string5[0]) - 1) << std::endl;

  obj.Serialize(buffer);
  buffer.push_back('\0');
  std::cout << buffer.data() << std::endl;

  std::cin.get();
  return 0;
}
