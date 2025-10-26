#pragma once

namespace GParsing {
	template <typename CharT>
	class GPARSING_API HTMLArgument
	{
	private:
		std::vector<CharT> m_key;
		std::vector<CharT> m_value;
	public:
    void SetKey(const CharT* _name, const size_t _nameLen) noexcept {
      m_key.resize(_nameLen);

      for (size_t index = 0; index < _nameLen; index++)
      {
        m_key[index] = _name[index];
      }
    }

    void SetKey(const std::string& _name) noexcept {
      m_key.clear();
      m_key.reserve(_name.length());

      for (const CharT& c : _name)
      {
        m_key.push_back(c);
      }
    }

    const CharT* const GetKey() const noexcept {
      return m_key.data();
    }

    const size_t GetKeySize() const {
      return m_key.size();
    }

    void SetValue(const CharT* _value, const size_t _valueLen) noexcept {
      m_value.resize(_valueLen);

      for (size_t index = 0; index < _valueLen; index++)
      {
        m_value[index] = _value[index];
      }
    }

    void SetValue(const std::string& _value) noexcept {
      m_value.clear();
      m_value.reserve(_value.length());

      for (const CharT& c : _value)
      {
        m_value.push_back(c);
      }
    }

    const CharT* const GetValue() const noexcept {
      return m_value.data();
    }

    const size_t GetValueSize() const {
      return m_value.size();
    }
	};

}