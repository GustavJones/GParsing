#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/HTML/HTMLToken.hpp"
#include "GParsing/HTML/HTMLElement.hpp"
#include "GParsing/HTML/HTMLDoctype.hpp"
#include "GParsing/Core/Compare.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace GParsing {
const std::vector<std::string> HTML_VOID_ELEMENTS = { "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr" };
const std::vector<std::string> HTML_RAW_TEXT_PARSE_ELEMENTS = {"script", "style", "title", "textarea"};

template <typename CharT>
GPARSING_API class HTMLDocument {
private:
  HTMLDoctype m_doctypeToken;
  HTMLElement<CharT> * m_rootElement;

  enum class States {
    TextOnly,
    Text,
    Tag,
    StartTag,
    EndTag,
    DOCTYPE,
    Comment,
    AttributeKey,
    AttributeValueSingleQuoted,
    AttributeValueDoubleQuoted,
    EndOfFile,
  };

  const std::vector<HTMLToken<CharT>> _Tokenize(const CharT *const _buffer, const size_t _bufferLen, const std::vector<std::string> &_voidElements, const std::vector<std::string> &_rawTextElements) const {
    States state;
    std::vector<HTMLToken<CharT>> tokens;
    HTMLToken<CharT> token;
    size_t index = 0;
    state = States::Text;

    if (_bufferLen == 0) {
      throw std::runtime_error("Empty buffer");
    }

    while (state != States::EndOfFile && index < _bufferLen) {
      char c = _buffer[index];

      switch (state) {
      case States::Comment:
        switch (c)
        {
        case '>':
          if (_buffer[index - 1] == '-' && _buffer[index - 2] == '-')
          {
            state = States::Text;
            token.Type() = HTMLTokenType::Comment;
            token.SetCharacter(token.Size() - 1, ' ');
            token.SetCharacter(token.Size() - 2, ' ');
            token.RemoveSurroundingSpaces();
            tokens.push_back(token);

            token = HTMLToken<CharT>();
          }

          break;
        default:
          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);
          break;
        }
        break;

      case States::TextOnly:
        switch (c) {
        case '<':
          if (_buffer[index + 1] != '/')
          {
            token.IncrementSize();
            token.SetCharacter(token.Size() - 1, c);
            break;
          }

          index++;

          state = States::EndTag;

          if (token.Size() > 0)
          {
            token.Type() = HTMLTokenType::Text;
            token.RemoveSurroundingSpaces();
            tokens.push_back(token);

            token = HTMLToken<CharT>();
          }

          break;

        default:
          if (IsWhitespace(c) && token.Size() <= 0)
          {
            break;
          }

          if (c == '\0' || index == _bufferLen - 1)
          {
            state = States::EndOfFile;

            if (token.Size() > 0)
            {
              token.Type() = HTMLTokenType::Text;
              tokens.push_back(token);

              token = HTMLToken<CharT>();
            }
          }

          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);

          break;
        }
        break;

      case States::Text:
        switch (c) {
        case '<':
          state = States::Tag;

          if (token.Size() > 0)
          {
            token.Type() = HTMLTokenType::Text;
            token.RemoveSurroundingSpaces();
            tokens.push_back(token);

            token = HTMLToken<CharT>();
          }

          break;

        default:
          if (IsWhitespace(c) && token.Size() <= 0)
          {
            break;
          }

          if (c == '\0' || index == _bufferLen - 1)
          {
            state = States::EndOfFile;

            if (token.Size() > 0)
            {
              token.Type() = HTMLTokenType::Text;
              tokens.push_back(token);

              token = HTMLToken<CharT>();
            }
          }

          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);

          break;
        }
        break;

      case States::Tag:
        switch (c) {
        case '!':
          if (_buffer[index + 1] == '-' && _buffer[index + 2] == '-')
          {
            state = States::Comment;
            index += 2;

            break;
          }

          state = States::DOCTYPE;
          c = _buffer[++index];

          {
            const std::string doctypeIdentifier = "DOCTYPE";

            for (size_t i = 0; i < doctypeIdentifier.length(); i++) {
              if (!CaseUnsensitiveMatch(c, doctypeIdentifier[i])) {
                throw std::runtime_error(
                    "Doctype start tag found but string missing");
              }

              c = _buffer[++index];
            }

            if (!IsWhitespace(c))
            {
              throw std::runtime_error("No whitespace after Doctype identifier");
            }

            index--;
          }

          break;
        case '/':
          state = States::EndTag;
          break;

        default:
          if (IsWhitespace(c))
          {
            break;
          }

          state = States::StartTag;
          index--;
          break;
        }
        break;

      case States::DOCTYPE:
        switch (c) {
          case '>':
            state = States::Text;
            token.Type() = HTMLTokenType::DOCTYPE;
            token.RemoveSurroundingSpaces();

            tokens.push_back(token);
            token = HTMLToken<CharT>();
            break;
          default:
            token.IncrementSize();
            token.SetCharacter(token.Size() - 1, c);
            break;
        }
        break;

      case States::StartTag:
        switch (c)
        {
          case '>':

            state = States::Text;
            token.Type() = HTMLTokenType::StartTag;
            token.RemoveSurroundingSpaces();

            for (const auto& rawTextElement : _rawTextElements)
            {
              if (CaseUnsensitiveMatch((CharT *)rawTextElement.c_str(), token.Value(), rawTextElement.length()))
              {
                state = States::TextOnly;
              }
            }

            tokens.push_back(token);
            token = HTMLToken<CharT>();
            break;

          case '/':
            token.AddFlag(HTMLTokenFlags::StartTagAutoClosing);
            break;

          case '=':
          {
            const size_t delimiterIndex = index;

            bool stop = false;
            bool found = false;
            while (!stop && index >= 0)
            {
              index--;

              if (!IsWhitespace(_buffer[index]))
              {
                found = true;
              }

              if (IsWhitespace(_buffer[index]) && found)
              {
                stop = true;
              }
            }

            const size_t attributeSize = delimiterIndex - index;

            token.Erase(token.Size() - attributeSize, attributeSize);

            state = States::AttributeKey;

            token.Type() = HTMLTokenType::StartTag;
            token.RemoveSurroundingSpaces();

            tokens.push_back(token);
            token = HTMLToken<CharT>();
            break;
          }
          default:
            token.IncrementSize();
            token.SetCharacter(token.Size() - 1, c);
            break;
        }
        break;
      
      case States::EndTag:
        switch (c)
        {
        case '>':
          state = States::Text;

          token.Type() = HTMLTokenType::EndTag;
          token.RemoveSurroundingSpaces();
          tokens.push_back(token);

          token = HTMLToken<CharT>();
          break;
        default:
          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);
          break;
        }
        break;

      case States::AttributeKey:
        switch (c)
        {
        case '\'':
        {
          state = States::AttributeValueSingleQuoted;

          for (int64_t i = token.Size() - 1; i >= 0; i--)
          {
            if (token.GetCharacter(i) == '=')
            {
              token.SetCharacter(i, ' ');
            }

            if (!IsWhitespace(token.GetCharacter(i)))
            {
              break;
            }
          }

          token.Type() = HTMLTokenType::Attribute;
          token.RemoveSurroundingSpaces();
          tokens.push_back(token);

          token = HTMLToken<CharT>();
          break;
        }
        case '"':
          state = States::AttributeValueDoubleQuoted;

          for (int64_t i = token.Size() - 1; i >= 0; i--)
          {
            if (token.GetCharacter(i) == '=')
            {
              token.SetCharacter(i, ' ');
            }

            if (!IsWhitespace(token.GetCharacter(i)))
            {
              break;
            }
          }

          token.Type() = HTMLTokenType::Attribute;
          token.RemoveSurroundingSpaces();
          tokens.push_back(token);

          token = HTMLToken<CharT>();
          break;

        case '/':
        {
          index++;
          while (IsWhitespace(_buffer[index]))
          {
            index++;
          }

          if (_buffer[index] != '>')
          {
            throw std::runtime_error("Arguments following self closing tag symbol or tag not closed");
          }

          int64_t tokenIndex = tokens.size() - 1;
          while ((tokens[tokenIndex].Type() == HTMLTokenType::Attribute || tokens[tokenIndex].Type() == HTMLTokenType::AttributeValue) && tokenIndex >= 0)
          {
            tokenIndex--;
          }

          //if (tokenIndex < 0 || tokens[tokenIndex].Type() != HTMLTokenType::StartTag)
          //{
          //  throw std::runtime_error("Error in token order. Start tag not before Attribute tokens.");
          //}

          if (tokenIndex >= 0 && tokens[tokenIndex].Type() == HTMLTokenType::StartTag)
          {
            tokens[tokenIndex].AddFlag(HTMLTokenFlags::StartTagAutoClosing);
          }

          state = States::Text;

          break;
        }
        case '>':
        {
          state = States::Text;

          int64_t tokenIndex = tokens.size() - 1;
          while ((tokens[tokenIndex].Type() == HTMLTokenType::Attribute || tokens[tokenIndex].Type() == HTMLTokenType::AttributeValue) && tokenIndex >= 0)
          {
            tokenIndex--;
          }

          //if (tokenIndex < 0 || tokens[tokenIndex].Type() != HTMLTokenType::StartTag)
          //{
          //  throw std::runtime_error("Error in token order. Start tag not before Attribute tokens.");
          //}

          if (tokenIndex >= 0 && tokens[tokenIndex].Type() == HTMLTokenType::StartTag)
          {
            for (const auto& rawTextElement : _rawTextElements)
            {
              if (CaseUnsensitiveMatch((CharT *)rawTextElement.c_str(), tokens[tokenIndex].Value(), rawTextElement.length()))
              {
                state = States::TextOnly;
              }
            }
          }

          break;
        }
        default:
          if (IsWhitespace(c) && token.Size() <= 0)
          {
            break;
          }

          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);
          break;
        }
        break;

      case States::AttributeValueSingleQuoted:
        switch (c)
        {
        case '\'':
          state = States::AttributeKey;
          token.Type() = HTMLTokenType::AttributeValue;
          token.RemoveSurroundingSpaces();
          tokens.push_back(token);

          token = HTMLToken<CharT>();
          break;

        default:
          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);
          break;
        }
        break;

      case States::AttributeValueDoubleQuoted:
        switch (c)
        {
        case '"':
          state = States::AttributeKey;
          token.Type() = HTMLTokenType::AttributeValue;
          token.RemoveSurroundingSpaces();
          tokens.push_back(token);

          token = HTMLToken<CharT>();
          break;

        default:
          token.IncrementSize();
          token.SetCharacter(token.Size() - 1, c);
          break;
        }
        break;
      }

      index++;
    }

    for (size_t index = 0; index < tokens.size(); index++)
    {
      std::string tokenName;

      for (size_t tokenIndex = 0; tokenIndex < tokens[index].Size(); tokenIndex++)
      {
        tokenName += tokens[index].GetCharacter(tokenIndex);
      }

      for (size_t voidElementIndex = 0; voidElementIndex < _voidElements.size(); voidElementIndex++)
      {
        if (tokenName == _voidElements[voidElementIndex])
        {
          tokens[index].AddFlag(HTMLTokenFlags::StartTagAutoClosing);
        }
      }
    }

    return tokens;
  }

  void _BuildTree(const std::vector<HTMLToken<CharT>> &_tokens) {
    bool foundDoctype = false;
    size_t doctypeTokenIndex = 0, rootTokenIndex = 0;

    if (_tokens.size() <= 0)
    {
      throw std::runtime_error("Empty token array");
    }

    for (size_t index = 0; index < _tokens.size(); index++)
    {
      const HTMLToken<CharT> &token = _tokens[index];

      if (token.Type() == HTMLTokenType::DOCTYPE)
      {
        foundDoctype = true;
        m_doctypeToken.Parse(token);
        doctypeTokenIndex = index;
        break;
      }
    }

    if (m_doctypeToken.root == "")
    {
      m_doctypeToken.root = "html";
    }

    for (size_t index = 0; index < _tokens.size(); index++)
    {
      const HTMLToken<CharT>& token = _tokens[index];
      std::string tokenString;

      for (size_t tokenIndex = 0; tokenIndex < token.Size(); tokenIndex++)
      {
        tokenString += token.GetCharacter(tokenIndex);
      }

      if (token.Type() == HTMLTokenType::StartTag && tokenString == m_doctypeToken.root)
      {
        if (index >= doctypeTokenIndex)
        {
          rootTokenIndex = index;
        }
        break;
      }
    }

    delete m_rootElement;
    m_rootElement = new HTMLElement<CharT>();
    m_rootElement->ParseFromTokens(_tokens, rootTokenIndex);
  }

  void _SerializeElement(std::vector<CharT>& _buffer, const HTMLElement<CharT>* const _element, const size_t _indentLevel, const CharT* const _indentCharacters, const size_t _indentCharactersLen) const {
    for (size_t indent = 0; indent < _indentLevel; indent++)
    {
      for (size_t indentIndex = 0; indentIndex < _indentCharactersLen; indentIndex++)
      {
        _buffer.push_back(_indentCharacters[indentIndex]);
      }
    }

    _buffer.push_back('<');

    for (size_t nameIndex = 0; nameIndex < _element->GetNameSize(); nameIndex++)
    {
      _buffer.push_back(_element->GetName()[nameIndex]);
    }

    if (_element->GetArgumentCount() > 0)
    {
      for (size_t argumentIndex = 0; argumentIndex < _element->GetArgumentCount(); argumentIndex++)
      {
        const HTMLArgument<CharT>* argument = _element->GetArgument(argumentIndex);

        _buffer.push_back(' ');

        for (size_t keyIndex = 0; keyIndex < argument->GetKeySize(); keyIndex++)
        {
          _buffer.push_back(argument->GetKey()[keyIndex]);
        }

        _buffer.push_back('=');
        _buffer.push_back('\"');

        for (size_t valueIndex = 0; valueIndex < argument->GetValueSize(); valueIndex++)
        {
          _buffer.push_back(argument->GetValue()[valueIndex]);
        }

        _buffer.push_back('\"');
      }
    }

    if (_element->GetAutoClosing())
    {
      _buffer.push_back('/');
    }

    _buffer.push_back('>');

    for (size_t valueIndex = 0; valueIndex < _element->GetValuesAmount(); valueIndex++)
    {
      _buffer.push_back('\n');

      bool isChildObject = false;
      const CharT *const value = _element->GetValueAtIndex(valueIndex);
      const size_t valueLen = _element->GetValueSizeAtIndex(valueIndex);

      if (valueLen <= 0)
      {
        continue;
      }

      if (value[0] == '\0')
      {
        isChildObject = true;
      }

      if (!isChildObject)
      {
        for (size_t indent = 0; indent < _indentLevel; indent++)
        {
          for (size_t indentIndex = 0; indentIndex < _indentCharactersLen; indentIndex++)
          {
            _buffer.push_back(_indentCharacters[indentIndex]);
          }
        }

        for (size_t index = 0; index < valueLen; index++)
        {
          _buffer.push_back(value[index]);
        }
      }



      else
      {
        std::string childIndexString;

        for (size_t index = 1; index < valueLen; index++)
        {
          childIndexString += value[index];
        }

        size_t childIndex = std::stoul(childIndexString);

        _SerializeElement(_buffer, _element->GetChild(childIndex), _indentLevel + 1, _indentCharacters, _indentCharactersLen);
      }
    }

    if (!_element->GetAutoClosing())
    {
      _buffer.push_back('\n');

      for (size_t indent = 0; indent < _indentLevel; indent++)
      {
        for (size_t indentIndex = 0; indentIndex < _indentCharactersLen; indentIndex++)
        {
          _buffer.push_back(_indentCharacters[indentIndex]);
        }
      }

      _buffer.push_back('<');
      _buffer.push_back('/');

      for (size_t nameIndex = 0; nameIndex < _element->GetNameSize(); nameIndex++)
      {
        _buffer.push_back(_element->GetName()[nameIndex]);
      }
      _buffer.push_back('>');
    }
  }

public:
  HTMLDocument() noexcept : m_rootElement(nullptr) {};

  HTMLDocument(HTMLDocument&& _doc) noexcept : m_rootElement(_doc.m_rootElement), m_doctypeToken(_doc.m_doctypeToken) {
    _doc.m_rootElement = nullptr;
    _doc.m_doctypeToken = HTMLDoctype();
  }

  HTMLDocument(const HTMLDocument& _doc) noexcept : m_doctypeToken(_doc.m_doctypeToken), m_rootElement(_doc.m_rootElement ? new HTMLElement<CharT>(*_doc.m_rootElement) : nullptr) {  }

  HTMLDocument& operator=(HTMLDocument&& _doc) noexcept {
    if (this == &_doc)
    {
      return *this;
    }

    delete m_rootElement;
    m_rootElement = _doc.m_rootElement;
    _doc.m_rootElement = nullptr;

    m_doctypeToken = _doc.m_doctypeToken;
    _doc.m_doctypeToken = HTMLDoctype();

    return *this;
  }

  HTMLDocument& operator=(const HTMLDocument& _doc) noexcept {
    if (this == &_doc)
    {
      return *this;
    }

    delete m_rootElement;
    m_rootElement = _doc.m_rootElement ? new HTMLElement<CharT>(*_doc.m_rootElement) : nullptr;

    m_doctypeToken = _doc.m_doctypeToken;

    return *this;
  }

  ~HTMLDocument() noexcept {
    delete m_rootElement;
  }

  void Parse(const CharT *const _buffer, const size_t _bufferLen, const std::vector<std::string> &_voidElements = GParsing::HTML_VOID_ELEMENTS, const std::vector<std::string>& _rawTextElements = GParsing::HTML_RAW_TEXT_PARSE_ELEMENTS) {
    std::vector<HTMLToken<CharT>> tokens;
    tokens = _Tokenize(_buffer, _bufferLen, _voidElements, _rawTextElements);

    //for (size_t index = 0; index < tokens.size(); index++)
    //{
    //  std::cout << '\"';
    //  for (size_t tokenCharIndex = 0; tokenCharIndex < tokens[index].Size(); tokenCharIndex++)
    //  {
    //    std::cout << tokens[index].GetCharacter(tokenCharIndex);
    //  }

    //  std::cout << '\"';

    //  std::cout << '\t' << "Type: ";

    //  std::cout << (uint32_t)tokens[index].Type();

    //  std::cout << '\t' << "Flags:";
    //  for (const auto& flag : tokens[index].GetFlags())
    //  {
    //    std::cout << ' ' << (uint32_t)flag;
    //  }
    //  std::cout << std::endl;
    //}

    _BuildTree(tokens);
  }

  std::vector<CharT> Serialize(const CharT *const _indentCharacters, const size_t _indentCharactersLen) const {
    std::vector<CharT> output;

    if (m_doctypeToken.IsValid())
    {
      const std::string doctypeIdentifier = "<!DOCTYPE";

      for (const CharT& c: doctypeIdentifier)
      {
        output.push_back(c);
      }

      output.push_back(' ');

      for (const CharT& c : m_doctypeToken.root)
      {
        output.push_back(c);
      }

      output.push_back(' ');

      for (const CharT& c : m_doctypeToken.publicity)
      {
        output.push_back(c);
      }

      output.push_back(' ');

      for (const CharT& c : m_doctypeToken.declaration)
      {
        output.push_back(c);
      }

      output.push_back('>');
      output.push_back('\n');
    }

    const HTMLElement<CharT>* root = GetRootElement();
    _SerializeElement(output, root, 0, _indentCharacters, _indentCharactersLen);

    output.push_back('\0');

    return output;
  }

  HTMLElement<CharT>* GetRootElement() {
    return m_rootElement;
  }

  const HTMLElement<CharT>* GetRootElement() const {
    return m_rootElement;
  }

  HTMLDoctype& GetDoctype() { return m_doctypeToken; }

  const HTMLDoctype& GetDoctype() const { return m_doctypeToken; }
};
} // namespace GParsing
