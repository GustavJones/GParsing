#pragma once
#include "GParsing/Core/API.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace GParsing {

enum class HTMLTokenFlags {
  StartTagAutoClosing,
};

enum class HTMLTokenType {
  UNKNOWN,
  DOCTYPE,
  StartTag,
  EndTag,
  Text,
  Comment,
  Attribute,
  AttributeValue
};

template <typename CharT> GPARSING_API class HTMLToken {
private:
  std::vector<HTMLTokenFlags> m_flags;
  HTMLTokenType m_type;
  std::vector<CharT> m_value;

public:
  HTMLToken<CharT>() : m_type(HTMLTokenType::UNKNOWN), m_value(0), m_flags(0) {}

  bool AddFlag(const HTMLTokenFlags& _flag) {
    for (const auto& flag : m_flags)
    {
      if (_flag == flag)
      {
        return false;
      }
    }

    m_flags.push_back(_flag);
    return true;
  }

  bool RemoveFlag(const HTMLTokenFlags& _flag) {
    bool found = false;
    size_t removeIndex = 0;

    for (size_t index = 0; index < m_flags.size(); index++)
    {
      if (_flag == m_flags[index])
      {
        removeIndex = index;
        found = true;
      }
    }

    if (found)
    {
      m_flags.erase(m_flags.begin() + removeIndex, m_flags.begin() + removeIndex + 1);
      return true;
    }
    else
    {
      return false;
    }
  }

  const std::vector<HTMLTokenFlags>& GetFlags() const {
    return m_flags;
  }

  HTMLTokenType &Type() { return m_type; }
  const HTMLTokenType &Type() const { return m_type; }

  CharT *const Value() { return m_value.data(); }
  const CharT *const Value() const { return m_value.data(); }

  void SetCharacter(const size_t _index, const CharT _char) {
    if (m_value.size() == 0) {
      throw std::runtime_error("Out of bounds");
    }

    if (_index > m_value.size() - 1) {
      throw std::runtime_error("Out of bounds");
    }

    m_value[_index] = _char;
  }

  const CharT GetCharacter(const size_t _index) const {
    if (_index > m_value.size() - 1) {
      throw std::runtime_error("Out of bounds");
    }

    if (m_value.size() == 0) {
      throw std::runtime_error("Out of bounds");
    }

    return m_value[_index];
  }

  const size_t Size() const { return m_value.size(); }
  void Resize(const size_t _size) { m_value.resize(_size); }
  void IncrementSize() { m_value.emplace_back(); }
  void DecrementSize() { m_value.erase(m_value.size() - 1); }

  void RemoveSurroundingSpaces() {
    for (size_t i = 0; i < Size(); i++)
    {
      if (GetCharacter(i) != ' ')
      {
        if (i > 0)
        {
          Erase(0, i);
          break;
        }
        else
        {
          break;
        }
      }
    }

    for (int64_t i = Size() - 1; i >= 0; i--)
    {
      if (GetCharacter(i) != ' ')
      {
        Erase(i + 1, Size() - i - 1);
        break;
      }
    }
  }

  void CopyInto(const CharT *const _buffer, const size_t _bufferLen) {
    if (_bufferLen > m_value.size()) {
      throw std::runtime_error("Copy buffer to large for current value");
    }

    for (size_t index = 0; index < _bufferLen; index++) {
      SetCharacter(index, _buffer[index]);
    }
  }

  void Erase(const size_t _index, const size_t _count) {
    if (_index + _count > m_value.size())
    {
      throw std::runtime_error("Erase out of bounds");
    }

    m_value.erase(m_value.begin() + _index, m_value.begin() + _index + _count);
  }
};
} // namespace GParsing
