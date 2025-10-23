#pragma once
#include <cstdint>
#include <vector>

namespace GParsing {
  template <typename CharT>
  bool CaseSensitiveMatch(const CharT _char, const CharT _compare) {
    if (_char == _compare) {
      return true;
    }
    else {
      return false;
    }
  }

  template <typename CharT>
  bool CaseUnsensitiveMatch(const CharT _char, const CharT _compare) {
    constexpr const uint32_t AMOUNT_BETWEEN_CASES = 32;

    // Between 'A' and 'Z'
    if (_char >= 'A' && _char <= 'Z') {
      if (_char == _compare || (_char + AMOUNT_BETWEEN_CASES) == _compare) {
        return true;
      }
      else {
        return false;
      }
    }

    // Between 'a' and 'z'
    else if (_char >= 'a' && _char <= 'z') {
      if (_char == _compare || (_char - AMOUNT_BETWEEN_CASES) == _compare) {
        return true;
      }
      else {
        return false;
      }
    }
    // Other characters
    else {
      if (_char == _compare) {
        return true;
      }
      else {
        return false;
      }
    }
  }

  template <typename CharT>
  bool CaseSensitiveMatch(const CharT* const _char, const CharT* const _compare, const size_t _len) {
    for (size_t index = 0; index < _len; index++)
    {
      const CharT first = _char[index];
      const CharT second = _compare[index];

      if (!GParsing::CaseSensitiveMatch<CharT>(first, second))
      {
        return false;
      }
    }

    return true;
  }

  template <typename CharT>
  bool CaseUnsensitiveMatch(const CharT* const _char, const CharT* const _compare, const size_t _len) {
    for (size_t index = 0; index < _len; index++)
    {
      const CharT first = _char[index];
      const CharT second = _compare[index];

      if (!GParsing::CaseUnsensitiveMatch<CharT>(first, second))
      {
        return false;
      }
    }

    return true;
  }

  template <typename CharT>
  bool IsWhitespace(const CharT _char, const std::vector<CharT> &_whitespaces = { '\u0009', '\u000A', '\u000C', '\u000D', '\u0020' }) {
    for (size_t i = 0; i < _whitespaces.size(); i++)
    {
      if (GParsing::CaseUnsensitiveMatch<CharT>(_char, _whitespaces[i]))
      {
        return true;
      }
    }

    return false;
  }
}
