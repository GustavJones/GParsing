#pragma once
#include "GParsing/HTTP/HTTPString.hpp"
#include <vector>

namespace GParsing {
	template<typename CharT>
	class HTTPHeader {
	private:
		HTTPString<CharT> m_tag;
		HTTPString<CharT> m_value;
	public:
		HTTPHeader() = default;
		HTTPHeader(const HTTPString<CharT> &_tag, const HTTPString<CharT> &_value): m_tag(_tag), m_value(_value) {}

		bool IsEmpty() { return m_tag.empty(); }

		const HTTPString<CharT> &GetTag() const { return m_tag; }
		const HTTPString<CharT> &GetValue() const { return m_value; }

		void SetTag(const HTTPString<CharT> &_tag) { m_tag = _tag; }
		void SetValue(const HTTPString<CharT> &_value) { m_value = _value; }
	};

	template<typename CharT>
	class HTTPHeaderList {
	private:
		std::vector<HTTPHeader<CharT>> m_headers;

	public:
		void Add(const HTTPHeader<CharT> &_header) {
			m_headers.push_back(_header);
		}

		size_t Size() const { return m_headers.size(); }

		void Clear() { m_headers.clear(); }

		const HTTPHeader<CharT> &operator[](const size_t _index) const { return m_headers[_index]; }
		HTTPHeader<CharT> &operator[](const size_t _index) { return m_headers[_index]; }
	};
}
