#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Compare.hpp"
#include "GParsing/HTML/HTMLArgument.hpp"
#include <vector>
#include <string>

namespace GParsing {
template <typename CharT> GPARSING_API class HTMLElement
{
private:
  bool m_autoClosing;
  std::vector<CharT> m_name;
  std::vector<std::vector<CharT>> m_value;
  std::vector<HTMLArgument<CharT> *> m_arguments;
  std::vector<HTMLElement<CharT> *> m_children;

public:
  HTMLElement() noexcept : m_autoClosing(false), m_name(0), m_value(0), m_arguments(0), m_children(0) {}

  HTMLElement(const HTMLElement& _element) noexcept : m_autoClosing(_element.m_autoClosing), m_name(_element.m_name), m_value(_element.m_value), m_arguments(_element.m_arguments.size()), m_children(_element.m_children.size()) {
    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      m_arguments[i] = _element.m_arguments[i] ? new HTMLArgument<CharT>(*_element.m_arguments[i]) : nullptr;
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      m_children[i] = _element.m_children[i] ? new HTMLElement<CharT>(*_element.m_children[i]) : nullptr;
    }
  }

  HTMLElement(HTMLElement&& _element) noexcept : m_autoClosing(_element.m_autoClosing), m_name(_element.m_name), m_value(_element.m_value), m_arguments(_element.m_arguments.size()), m_children(_element.m_children.size()) {
    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      m_arguments[i] = _element.m_arguments[i];
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      m_children[i] = _element.m_children[i];
    }

    _element.m_arguments.clear();
    _element.m_children.clear();

    _element = HTMLElement<CharT>();
  }

  HTMLElement& operator=(const HTMLElement& _element) noexcept {
    if (&_element == this)
    {
      return *this;
    }

    m_autoClosing = _element.m_autoClosing;
    m_name = _element.m_name;
    m_value = _element.m_value;

    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      delete m_arguments[i];
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      delete m_children[i];
    }

    m_arguments.resize(_element.m_arguments.size());
    m_children.resize(_element.m_children.size());

    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      m_arguments[i] = _element.m_arguments[i] ? new HTMLArgument<CharT>(*_element.m_arguments[i]) : nullptr;
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      m_children[i] = _element.m_children[i] ? new HTMLElement<CharT>(*_element.m_children[i]) : nullptr;
    }

    return *this;
  }

  HTMLElement& operator=(HTMLElement&& _element) noexcept {
    if (&_element == this)
    {
      return *this;
    }

    m_autoClosing = _element.m_autoClosing;
    m_name = _element.m_name;
    m_value = _element.m_value;

    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      delete m_arguments[i];
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      delete m_children[i];
    }

    m_arguments.resize(_element.m_arguments.size());
    m_children.resize(_element.m_children.size());

    for (size_t i = 0; i < m_arguments.size(); i++)
    {
      m_arguments[i] = _element.m_arguments[i];
    }

    for (size_t i = 0; i < m_children.size(); i++)
    {
      m_children[i] = _element.m_children[i];
    }

    _element.m_arguments.clear();
    _element.m_children.clear();

    _element = HTMLElement<CharT>();

    return *this;
  }

  ~HTMLElement() noexcept {
    for (auto *element : m_children)
    {
      delete element;
    }

    for (auto *element : m_arguments)
    {
      delete element;
    }
  }

  void AddChild(HTMLElement<CharT> *const _child) noexcept {
    m_children.push_back(_child);
  }

  template<typename ...Args>
  void AddChild(Args... _args) {
    m_children.push_back(new HTMLElement<CharT>(_args...));
  }

  HTMLElement<CharT>* const GetChild(const size_t _index) { return m_children[_index]; }

  const HTMLElement<CharT>* const GetChild(const size_t _index) const { return m_children[_index]; }

  const size_t GetChildCount() const { return m_children.size(); }

  void AddArgument(HTMLArgument<CharT>* const _arg) noexcept {
    m_children.push_back(_arg);
  }

  template<typename ...Args>
  void AddArgument(Args... _args) {
    m_children.push_back(new HTMLArgument<CharT>(_args...));
  }

  HTMLArgument<CharT>* const GetArgument(const size_t _index) { return m_arguments[_index]; }

  const HTMLArgument<CharT>* const GetArgument(const size_t _index) const { return m_arguments[_index]; }

  const size_t GetArgumentCount() const { return m_arguments.size(); }

  void SetName(const CharT *_name, const size_t _nameLen) noexcept {
    m_name.resize(_nameLen);

    for (size_t index = 0; index < _nameLen; index++)
    {
      m_name[index] = _name[index];
    }
  }

  void SetName(const std::string& _name) noexcept {
    m_name.clear();
    m_name.reserve(_name.length());

    for (const CharT& c : _name)
    {
      m_name.push_back(c);
    }
  }

  const CharT *const GetName() const noexcept {
    return m_name.data();
  }

  const size_t GetNameSize() const {
    return m_name.size();
  }

  void SetValueAtIndex(const size_t _index, const CharT* _value, const size_t _valueLen) noexcept {
    m_value[_index].resize(_valueLen);

    for (size_t index = 0; index < _valueLen; index++)
    {
      m_value[_index][index] = _value[index];
    }
  }

  void SetValueAtIndex(const size_t _index, const std::string& _value) noexcept {
    m_value[_index].clear();
    m_value[_index].reserve(_value.length());

    for (const CharT& c : _value)
    {
      m_value[_index].push_back(c);
    }
  }

  const CharT* const GetValueAtIndex(const size_t _index) const noexcept {
    return m_value[_index].data();
  }

  const size_t GetValueSizeAtIndex(const size_t _index) const {
    return m_value[_index].size();
  }

  void AppendValue(const CharT* _value, const size_t _valueLen) {
    m_value.emplace_back();

    SetValueAtIndex(m_value.size() - 1, _value, _valueLen);
  }

  void AppendValue(const std::string& _value)
  {
    m_value.emplace_back();

    SetValueAtIndex(m_value.size() - 1, _value);
  }

  void SetValuesAmount(const size_t _size) {
    m_value.resize(_size);
  }

  const size_t GetValuesAmount() const {
    return m_value.size();
  }

  void SetAutoClosing(const bool _value) noexcept {
    m_autoClosing = _value;
  }

  bool GetAutoClosing() const noexcept {
    return m_autoClosing;
  }

  void ParseFromTokens(const std::vector<HTMLToken<CharT>>& _tokens, const size_t _startIndex = 0) {
    size_t end = 0;
    ParseFromTokens(_tokens, end, _startIndex);
  }

  void ParseFromTokens(const std::vector<HTMLToken<CharT>> &_tokens, size_t& _endIndex, const size_t _startIndex) {
    size_t tokenIndex;
    const HTMLToken<CharT> &token = _tokens[_startIndex];
    const std::vector<HTMLTokenFlags> &tokenFlags = token.GetFlags();

    // Tag Name
    SetName(token.Value(), token.Size());

    // Flags
    for (size_t flagIndex = 0; flagIndex < tokenFlags.size(); flagIndex++)
    {
      switch (tokenFlags[flagIndex])
      {
      case HTMLTokenFlags::StartTagAutoClosing:
        SetAutoClosing(true);
        break;
      default:
        break;
      }
    }

    tokenIndex = _startIndex + 1;

    // Build argument tree
    bool endOfTag = false;
    bool parsingArgument = false;
    while (tokenIndex < _tokens.size() && !endOfTag)
    {
      const HTMLToken<CharT>& token = _tokens[tokenIndex];

      switch (token.Type())
      {
      case HTMLTokenType::Attribute:
      {
        if (parsingArgument)
        {
          throw std::runtime_error("Incorrect amount of pairs of arguments");
        }

        parsingArgument = true;
        HTMLArgument<CharT>* argument = new HTMLArgument<CharT>();
        m_arguments.push_back(argument);

        argument->SetKey(token.Value(), token.Size());
        break;
      }
      case HTMLTokenType::AttributeValue:
      {
        if (!parsingArgument)
        {
          throw std::runtime_error("Incorrect amount of pairs of arguments");
        }

        parsingArgument = false;
        m_arguments[m_arguments.size() - 1]->SetValue(token.Value(), token.Size());
        break;
      }
      default:
        if (parsingArgument)
        {
          throw std::runtime_error("Incorrect amount of pairs of arguments");
        }

        endOfTag = true;
        tokenIndex--;
        break;
      }

      tokenIndex++;
    }

    if (GetAutoClosing())
    {
      _endIndex = _startIndex;
      return;
    }

    // Parse Children
    while (tokenIndex < _tokens.size())
    {
      const HTMLToken<CharT>& token = _tokens[tokenIndex];

      switch (token.Type())
      {
      case HTMLTokenType::Text:
        AppendValue(token.Value(), token.Size());
        break;
      case HTMLTokenType::StartTag:
      {
        std::string indexString;
        size_t end = 0;
        m_children.push_back(new HTMLElement<CharT>());
        indexString = std::to_string(m_children.size() - 1);
        AppendValue('\0' + indexString);

        HTMLElement<CharT>* const child = m_children[m_children.size() - 1];

        child->ParseFromTokens(_tokens, end, tokenIndex);

        tokenIndex = end;
        break;
      }
      case HTMLTokenType::EndTag:
      {
        if (CaseSensitiveMatch(token.Value(), GetName(), token.Size()))
        {
          _endIndex = tokenIndex;

          return;
        }

        break;
      }
      default:
        break;
      }

      tokenIndex++;
    }
  }
};
}
