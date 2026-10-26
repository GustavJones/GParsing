#pragma once
#include "GParsing/HTTP/HTTPString.hpp"

namespace GParsing {
	enum class HTTPWhiteSpace {
		Space = ' ',
		Tab = '\t'
	};

	enum class HTTPNewline {
		LF = '\n',
		CR = '\r',
	};

	template<typename CharT>
	bool HTTPIsWhitespace(const size_t _index, const HTTPString<CharT> &_buffer) noexcept {
		if (_index >= _buffer.Length())  return false;
		switch (_buffer[_index]) {
			case (CharT)HTTPWhiteSpace::Space:
			{
				return true;
				break;
			}
			case (CharT)HTTPWhiteSpace::Tab:
			{
				return true;
				break;
			}
			default:
			{
				return false;
				break;
			}
		}
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetWhitespaceString() noexcept {
		HTTPString<CharT> output;
		output.Append((CharT)HTTPWhiteSpace::Space);
		return output;
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetAllWhitespacesString() noexcept {
		HTTPString<CharT> output;
		output.Append((CharT)HTTPWhiteSpace::Space);
		output.Append((CharT)HTTPWhiteSpace::Tab);
		return output;
	}

	template<typename CharT>
	bool HTTPIsNewline(const size_t _index, const HTTPString<CharT> &_buffer) noexcept {
		if (_index >= _buffer.Length())  return false;
		switch (_buffer[_index]) {
			case (CharT)HTTPNewline::LF:
			{
				return true;
				break;
			}
			case (CharT)HTTPNewline::CR:
			{
				return true;
				break;
			}
			default:
			{
				return false;
				break;
			}
		};
	}

	template<typename CharT>
	size_t HTTPGetNewlineSize(const size_t _index, const HTTPString<CharT> &_buffer) noexcept {
		if (!HTTPIsNewline(_index, _buffer)) return 0;

		switch (_buffer[_index]) {
			case (CharT)HTTPNewline::LF:
			{
				return 1;
				break;
			}
			case (CharT)HTTPNewline::CR:
			{
				if (_index + 1 > _buffer.Length()) return 1;
				if (_buffer[_index + 1] == (CharT)HTTPNewline::LF) {
					return 2;
				}
				else {
					return 1;
				}
				break;
			}
		};

		return 0;
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetNewlineString() noexcept {
		HTTPString<CharT> output;
		output.Append((CharT)HTTPNewline::CR);
		output.Append((CharT)HTTPNewline::LF);
		return output;
	}

	template<typename CharT>
	HTTPString<CharT> HTTPGetAllNewlinesString() noexcept {
		HTTPString<CharT> output;
		output.Append((CharT)HTTPNewline::CR);
		output.Append((CharT)HTTPNewline::LF);
		return output;
	}
}
