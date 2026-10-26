#pragma once
#include "GParsing/HTTP/HTTPVersion.hpp"
#include "GParsing/HTTP/HTTPHeader.hpp"
#include "GParsing/HTTP/HTTPWhitespace.hpp"
#include <stdexcept>
#include <sstream>
#include <ios>

namespace GParsing {
	template<typename CharT>
	class HTTPBaseStructure {
	protected:
		enum class State {
			Error,
			Idle,
			Method,
			URI,
			Version,
			ResponseCode,
			ResponseMessage,
			HeaderTag,
			HeaderValue,
			Message,
		};

		HTTPVersion m_version = HTTPVersion::Unknown;
		HTTPHeaderList<CharT> m_headers;
		HTTPString<CharT> m_message;
	
	protected:
		static void SkipWhitespace(size_t &_index, const HTTPString<CharT> &_buffer) noexcept {
			while (_index < _buffer.Length() && HTTPIsWhitespace(_index, _buffer)) _index++;
		}

		virtual HTTPVersion ParseVersion(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) = 0;

		[[nodiscard]]
		static HTTPString<CharT> ParseHeaderTag(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
			const CharT VALUE_DELIMETER = ':';
			HTTPString<CharT> tag;

			SkipWhitespace(_index, _buffer);

			// Empty header
			if (HTTPIsNewline(_index, _buffer)) {
				_index += HTTPGetNewlineSize(_index, _buffer);

				_state = State::Message;
				return {};
			}

			if (_index >= _buffer.Length()) {
				_state = State::Idle;
				return {};
			}

			while (_index < _buffer.Length()) {
				if (_buffer[_index] != VALUE_DELIMETER && !HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
					tag.Append(_buffer[_index]);
				}
				else {
					if (_buffer[_index] == VALUE_DELIMETER) {
						_index++;
					}
					else {
						SkipWhitespace(_index, _buffer);
					}

					if (!_buffer[_index] == VALUE_DELIMETER) {
						_state = State::Error;
						return {};
					}

					_state = State::HeaderValue;
					return tag;
				}

				_index++;
			}

			_state = State::Error;
			return {};
		}

		[[nodiscard]]
		static HTTPString<CharT> ParseHeaderValue(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
			HTTPString<CharT> value;

			SkipWhitespace(_index, _buffer);

			while (_index < _buffer.Length()) {
				if (!HTTPIsNewline(_index, _buffer)) {
					value.Append(_buffer[_index]);
				}
				else {
					_index += HTTPGetNewlineSize(_index, _buffer);

					value.Trim(HTTPGetAllWhitespacesString<CharT>());

					if (!HTTPIsWhitespace(_index, _buffer)) {
						_state = State::HeaderTag;
						return value;
					}
					else {
						SkipWhitespace(_index, _buffer);
						value.Append(HTTPGetWhitespaceString<CharT>());
						value.Append(_buffer[_index]);
					}
				}

				_index++;
			}

			_state = State::Error;
			return {};
		}

		[[nodiscard]]
		static HTTPString<CharT> ParseMessage(State &_state, size_t &_index, const HTTPString<CharT> &_buffer, const bool _chunkedMessages) {
			HTTPString<CharT> message;
			std::wstring chunk;
			size_t chunkSize = 0;

			if (_chunkedMessages) {
				while (_index < _buffer.Length()) {
					if (HTTPIsNewline(_index, _buffer) || _buffer[_index] == ';') {
						if (_buffer[_index] == ';') {
							while (_index < _buffer.Length() && !HTTPIsNewline(_index, _buffer)) _index++;
						}

						_index += HTTPGetNewlineSize(_index, _buffer);

						chunkSize = std::stoul(chunk.data(), 0, 16);

						for (size_t i = 0; i < chunkSize; i++) {
							message.Append(_buffer[_index]);
							_index++;
						}

						chunk.clear();

						SkipWhitespace(_index, _buffer);
						if (HTTPIsNewline(_index, _buffer)) _index += HTTPGetNewlineSize(_index, _buffer);

						if (chunkSize == 0) {
							_state = State::HeaderTag;
							return message;
						}
					}

					chunk += _buffer[_index];
					_index++;
				}
			}
			else {
				while (_index < _buffer.Length()) {
					message.Append(_buffer[_index]);
					_index++;
				}

				_state = State::Idle;
				return message;
			}

			_state = State::Idle;
			return {};
		}

		virtual void SerializeVersion(State &_state, HTTPString<CharT> &_buffer) const = 0;

		void SerializeHeaders(State &_state, HTTPString<CharT> &_buffer) const {
			for (size_t i = 0; i < m_headers.Size(); i++) {
				_buffer.Append(m_headers[i].GetTag());
				_buffer.Append(':');
				_buffer.Append(HTTPGetWhitespaceString<CharT>());
				_buffer.Append(m_headers[i].GetValue());
				_buffer.Append(HTTPGetNewlineString<CharT>());
			}

			_buffer.Append(HTTPGetNewlineString<CharT>());
			_state = State::Message;
		}

		void SerializeMessage(State &_state, HTTPString<CharT> &_buffer, const bool _chunkedMessages) const {
			std::stringstream ss;
			ss << std::hex << m_message.Length();

			HTTPString<CharT> messageLenStr = ss.str();

			if (_chunkedMessages) {
				for (size_t i = 0; i < messageLenStr.Length(); i++) {
					_buffer.Append(messageLenStr[i]);
				}

				_buffer.Append(HTTPGetNewlineString<CharT>());

				for (size_t i = 0; i < m_message.Length(); i++) {
					_buffer.Append(m_message[i]);
				}

				_buffer.Append(HTTPGetNewlineString<CharT>());

				_buffer.Append('0');
				_buffer.Append(HTTPGetNewlineString<CharT>());
				_buffer.Append(HTTPGetNewlineString<CharT>());

				_state = State::Idle;
			}
			else {
				for (size_t i = 0; i < m_message.Length(); i++) {
					_buffer.Append(m_message[i]);
				}

				_state = State::Idle;
			}
		}

		bool UseChunkedTransferEncoding() const {
			for (size_t i = 0; i < this->m_headers.Size(); i++) {
				HTTPString<CharT> tag = m_headers[i].GetTag();
				HTTPString<CharT> value = m_headers[i].GetValue();

				if (!tag.CaseInsensitiveMatch("Transfer-Encoding")) continue; 
				if (value == "chunked") return true; 
			}

			return false;
		}

	public:
		const HTTPString<CharT> &GetMessage() const { return m_message; }
		HTTPVersion GetVersion() const { return m_version; }
	  const HTTPHeaderList<CharT> &GetHeaders() const { return m_headers; }

		virtual void Parse(const HTTPString<CharT> &_buffer) = 0;
		virtual void Serialize(HTTPString<CharT> &_buffer) const = 0;
		virtual void Clear() = 0;

		virtual ~HTTPBaseStructure() = default;
	};
}
