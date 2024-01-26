#pragma once
#include <string>

// Function for finding a string surrounded by any character
inline int FindString(const char *buffer, int bufferLen,
                      const std::string &string, int startIndex = 0,
                      int endIndex = -1) {
  int end;

  if (endIndex == -1) {
    end = bufferLen;
  } else if (endIndex > bufferLen) {
    end = bufferLen;
  } else {
    end = endIndex;
  }

  int compareIndex = 0;
  for (int i = startIndex; i < end; i++) {
    if (buffer[i] == string[compareIndex]) {
      if (compareIndex == string.length() - 1) {
        return i - string.length() + 1;
      }
      compareIndex++;
    } else {
      compareIndex = 0;
    }
  }

  return -1;
}

// Function for finding a string surrounded by spaces
inline int FindWord(const char *buffer, int bufferLen,
                    const std::string &string, int startIndex = 0,
                    int endIndex = -1) {
  int end;

  if (endIndex == -1) {
    end = bufferLen;
  } else if (endIndex > bufferLen) {
    end = bufferLen;
  } else {
    end = endIndex;
  }

  std::string modified = " " + string + " ";
  int compareIndex = 0;
  for (int i = startIndex; i < end; i++) {
    if (buffer[i] == modified[compareIndex]) {
      if (compareIndex == modified.length() - 1) {
        return i - modified.length() + 2;
      }
      compareIndex++;
    } else {
      compareIndex = 0;
    }
  }

  return -1;
}
