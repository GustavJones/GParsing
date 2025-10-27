#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Compare.hpp"
#include <stdexcept>

namespace GParsing {
  enum class BufferLocation : uint8_t
  {
    BEGINNING = 0b01,
    END = 0b10,
    BOTH = 0b100
  };

  enum class Endianness : uint8_t
  {
    BIG,
    LITTLE
  };

  template <typename CharT>
  GPARSING_API void RemoveCharacterPadding(std::vector<CharT>& _buffer, const std::vector<CharT>& _characters, BufferLocation _location = BufferLocation::BOTH) {
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
  GPARSING_API std::vector<CharT> RemoveCharacterPadding(const CharT *const _buffer, const size_t _bufferLen, const std::vector<CharT>& _characters, BufferLocation _location = BufferLocation::BOTH) {
    std::vector<CharT> buffer;
    buffer.resize(_bufferLen);

    for (size_t i = 0; i < _bufferLen; i++)
    {
      buffer[i] = _buffer[i];
    }

    RemoveCharacterPadding(buffer, _characters, _location);
    return buffer;
  }

  template <typename CharT, typename BytesT>
  GPARSING_API BytesT BytesFromBuffer(const std::vector<CharT>& _buffer, const size_t _index, const Endianness _endianness = Endianness::BIG) {
    const size_t bytesTSize = sizeof(BytesT) * 8;
    const size_t charTSize = sizeof(CharT) * 8;
    BytesT output = 0;

    if (bytesTSize % charTSize != 0)
    {
      throw std::runtime_error("Incompatable sizes");
    }

    size_t readAmount = bytesTSize / charTSize;

    if (readAmount > _buffer.size())
    {
      throw std::runtime_error("Not enough memory in buffer");
    }

    if (_endianness == Endianness::BIG)
    {
      for (size_t i = 0; i < readAmount; i++)
      {
        BytesT unshifted = static_cast<BytesT>(_buffer[_index + i]);
        size_t shiftAmount = bytesTSize - ((i + 1) * charTSize);
        output |= unshifted << shiftAmount;
      }
    }
    else if (_endianness == Endianness::LITTLE)
    {
      for (size_t i = 0; i < readAmount; i++)
      {
        BytesT unshifted = static_cast<BytesT>(_buffer[_index + i]);
        size_t shiftAmount = i * charTSize;
        output |= unshifted << shiftAmount;
      }
    }
    else
    {
      throw std::runtime_error("Unknown endianness");
    }

    return output;
  }

  template <typename CharT, typename BytesT>
  GPARSING_API std::vector<CharT> BufferFromBytes(const BytesT& _bytes, const Endianness _endianness = Endianness::BIG) {
    const size_t bytesTSize = sizeof(BytesT) * 8;
    const size_t charTSize = sizeof(CharT) * 8;
    std::vector<CharT> output;

    if (bytesTSize % charTSize != 0)
    {
      throw std::runtime_error("Incompatable sizes");
    }

    size_t writeAmount = bytesTSize / charTSize;
    output.resize(writeAmount);

    if (_endianness == Endianness::BIG)
    {
      for (size_t i = 0; i < writeAmount; i++)
      {
        size_t shiftAmount = bytesTSize - ((i + 1) * charTSize);
        output[i] = _bytes >> shiftAmount;
      }
    }
    else if (_endianness == Endianness::LITTLE)
    {
      for (size_t i = 0; i < writeAmount; i++)
      {
        size_t shiftAmount = i * charTSize;
        output[i] = _bytes >> shiftAmount;
      }
    }
    else
    {
      throw std::runtime_error("Unknown endianness");
    }

    return output;
  }
}