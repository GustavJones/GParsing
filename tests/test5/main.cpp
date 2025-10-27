#include "GParsing/GParsing.hpp"
#include "GParsing/Core/Buffers.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<unsigned char> buffer;

  std::fstream f;
  f.open("test5.JPG", std::ios::in | std::ios::binary | std::ios::ate);

  if (f.is_open())
  {
    buffer.resize(f.tellg());
    f.seekg(f.beg);
    f.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
  }

  f.close();

  GParsing::BytesFromBuffer<unsigned char, uint16_t>(buffer, 0);

  GParsing::EXIFImageJPEG image;
  std::cout << image.Parse(buffer) << std::endl;

  std::cin.get();
  return 0;
}