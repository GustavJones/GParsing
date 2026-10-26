#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <iostream>

namespace GParsing {
	template<typename CharT>
	class HTTPString {
	private:
		std::vector<CharT> m_data;
	public:
		HTTPString() = default;
		HTTPString(const std::vector<CharT> &_str) : m_data(_str) {}

		HTTPString(const char _str[]) {
			std::string stdString = _str;

			m_data.reserve(stdString.length());

			for (size_t i = 0; i < stdString.length(); i++) {
				m_data.push_back(stdString[i]);
			}
		}

		HTTPString(const std::string &_str) {
			m_data.reserve(_str.length());

			for (size_t i = 0; i < _str.length(); i++) {
				m_data.push_back(_str[i]);
			}
		}

		HTTPString(const std::wstring &_str) {
			m_data.reserve(_str.length());

			for (size_t i = 0; i < _str.length(); i++) {
				m_data.push_back(_str[i]);
			}
		}

		size_t Length() const { return m_data.size(); }

		void Erase(const size_t _index, const size_t _count = 1) { m_data.erase(m_data.begin() + _index, m_data.begin() + _index + _count); }

		void Append(const CharT _c) { m_data.push_back(_c); }
		void Append(const HTTPString &_str) { m_data.insert(m_data.end(), _str.m_data.begin(), _str.m_data.end()); }

		void Trim(const HTTPString<CharT> &_trimChars = " ") {
			for (size_t i = Length() - 1; i > 0; i--) {
				bool found = false;

				for (size_t j = 0; j < _trimChars.Length(); j++) {
					if (m_data[i] == _trimChars[j]) found = true;
				}

				if (found) {
					Erase(i);
				}
				else {
					break;
				}
			}

			for (size_t i = 0; i < Length(); i++) {
				bool found = false;

				for (size_t j = 0; j < _trimChars.Length(); j++) {
					if (m_data[i] == _trimChars[j]) found = true;
				}

				if (found) {
					Erase(i);
				}
				else {
					break;
				}
			}
		}

		bool CaseSensitiveMatch(const HTTPString &_cmp) const {
			if (_cmp.Length() != this->Length()) return false;

			for (size_t i = 0; i < this->Length(); i++) {
				if (_cmp[i] != this->operator[](i)) return false;
			}

			return true;
		}

		bool CaseInsensitiveMatch(const HTTPString &_cmp) const {
			if (_cmp.Length() != this->Length()) return false;

			for (size_t i = 0; i < this->Length(); i++) {
				if (_cmp[i] == this->operator[](i)) continue;

				if (_cmp[i] > 'A' && _cmp[i] < 'Z') {
					if (_cmp[i] == this->operator[](i) + ('a' - 'A')) return false;
				}
				else if (_cmp[i] > 'a' && _cmp[i] < 'z') {
					if (_cmp[i] != this->operator[](i) - ('a' - 'A')) return false;
				}
				else return false;
			}

			return true;
		}

		CharT &operator[](const size_t _index) { return m_data[_index]; }
		const CharT &operator[](const size_t _index) const { return m_data[_index]; }

		bool operator==(const HTTPString &_cmp) const { return this->CaseSensitiveMatch(_cmp); }
		bool operator!=(const HTTPString &_cmp) const { return !(this->CaseSensitiveMatch(_cmp)); }
	};
}

template<typename CharT>
std::ostream &operator<<(std::ostream &_stream, const GParsing::HTTPString<CharT> &_str) {
	for (size_t i = 0; i < _str.Length(); i++) {
		_stream << _str[i];
	}

	return _stream;
}
