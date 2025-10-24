#pragma once
#include "GParsing/Core/Compare.hpp"

namespace GParsing {
  enum class BufferLocation : uint8_t
  {
    BEGINNING = 0b01,
    END = 0b10,
    BOTH = 0b100
  };

  template <typename CharT>
  void RemoveCharacterPadding(std::vector<CharT>& _buffer, const std::vector<CharT>& _characters, BufferLocation _location = BufferLocation::BOTH) {
    if (_buffer.size() == 0)
    {
      return;
    }

    switch (_location)
    {
    case BufferLocation::BEGINNING:
    {
      for (size_t i = 0; i < _buffer.size(); i++)
      {
        bool found = false;

        for (size_t j = 0; j < _characters.size(); j++)
        {
          if (CaseSensitiveMatch(_buffer[i], _characters[j]))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          _buffer.erase(_buffer.begin(), _buffer.begin() + i);
          break;
        }
      }

      break;
    }
    case BufferLocation::END:
    {
      for (int64_t i = _buffer.size() - 1; i >= 0; i--)
      {
        bool found = false;

        for (size_t j = 0; j < _characters.size(); j++)
        {
          if (CaseSensitiveMatch(_buffer[i], _characters[j]))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          _buffer.erase(_buffer.begin() + i + 1, _buffer.end());
          break;
        }
      }

      break;
    }
    case BufferLocation::BOTH:
    {
      for (size_t i = 0; i < _buffer.size(); i++)
      {
        bool found = false;

        for (size_t j = 0; j < _characters.size(); j++)
        {
          if (CaseSensitiveMatch(_buffer[i], _characters[j]))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          _buffer.erase(_buffer.begin(), _buffer.begin() + i);
          break;
        }
      }

      for (int64_t i = _buffer.size() - 1; i >= 0; i--)
      {
        bool found = false;

        for (size_t j = 0; j < _characters.size(); j++)
        {
          if (CaseSensitiveMatch(_buffer[i], _characters[j]))
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          _buffer.erase(_buffer.begin() + i + 1, _buffer.end());
          break;
        }
      }

      break;
    }
    }

    return;
    
  }

  template <typename CharT>
  std::vector<CharT> RemoveCharacterPadding(const CharT *const _buffer, const size_t _bufferLen, const std::vector<CharT>& _characters, BufferLocation _location = BufferLocation::BOTH) {
    std::vector<CharT> buffer;
    buffer.resize(_bufferLen);

    for (size_t i = 0; i < _bufferLen; i++)
    {
      buffer[i] = _buffer[i];
    }

    RemoveCharacterPadding(buffer, _characters, _location);
    return buffer;
  }
}