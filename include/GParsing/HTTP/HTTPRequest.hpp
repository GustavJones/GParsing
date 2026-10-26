#pragma once
#include "GParsing/HTTP/HTTPBaseStructure.hpp"
#include "GParsing/HTTP/HTTPHeader.hpp"
#include "GParsing/HTTP/HTTPMethod.hpp"
#include "GParsing/HTTP/HTTPVersion.hpp"
#include "GParsing/HTTP/HTTPWhitespace.hpp"
#include <stdexcept>
#include <cstddef>

namespace GParsing {
	template<typename CharT>
	class HTTPRequest : public HTTPBaseStructure<CharT> {
	private:
		using typename HTTPBaseStructure<CharT>::State;

		HTTPMethod m_method = HTTPMethod::Unknown;
		HTTPString<CharT> m_uri;

	[[nodiscard]]
	static HTTPMethod ParseMethod(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
		HTTPString<CharT> method;

		HTTPBaseStructure<CharT>::SkipWhitespace(_index, _buffer);

		while (_index < _buffer.Length()) {
			if (!HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
				method.Append(_buffer[_index]);
			}
			else {
				HTTPBaseStructure<CharT>::SkipWhitespace(_index, _buffer);

				if (HTTPIsNewline(_index, _buffer)) {
					_state = State::Error;
					return HTTPMethod::Unknown;
				}

				_state = State::URI;
				return HTTPGetMethod(method);
			}

			_index++;
		}

		_state = State::Error;
		return HTTPMethod::Unknown;
	}

	[[nodiscard]]
	static HTTPString<CharT> ParseURI(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
		HTTPString<CharT> uri;

		HTTPBaseStructure<CharT>::SkipWhitespace(_index, _buffer);

		while (_index < _buffer.Length()) {
			if (!HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
				uri.Append(_buffer[_index]);
			}
			else {
				HTTPBaseStructure<CharT>::SkipWhitespace(_index, _buffer);

				if (HTTPIsNewline(_index, _buffer)) {
					_state = State::Error;
					return {};
				}

				_state = State::Version;
				return uri;
			}

			_index++;
		}

		_state = State::Error;
		return {};
	}

	[[nodiscard]]
	HTTPVersion ParseVersion(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) override {
		HTTPString<CharT> version;

		this->SkipWhitespace(_index, _buffer);

		while (_index < _buffer.Length()) {
			if (!HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
				version.Append(_buffer[_index]);
			}
			else {
				this->SkipWhitespace(_index, _buffer);

				if (!HTTPIsNewline(_index, _buffer)) { 
					_state = State::Error; 
					return HTTPVersion::Unknown;
				}
				else _index += HTTPGetNewlineSize(_index, _buffer);

				_state = State::HeaderTag;
				return HTTPGetVersion(version);
			}

			_index++;
		}

		_state = State::Error;
		return HTTPVersion::Unknown;
	}

	void SerializeMethod(State &_state, HTTPString<CharT> &_buffer) const {
		_buffer.Append(HTTPGetMethodString<CharT>(m_method));
		_buffer.Append(HTTPGetWhitespaceString<CharT>());

		_state = State::URI;
	}

	void SerializeURI(State &_state, HTTPString<CharT> &_buffer) const {
		_buffer.Append(m_uri);
		_buffer.Append(HTTPGetWhitespaceString<CharT>());

		_state = State::Version;
	}

	void SerializeVersion(State &_state, HTTPString<CharT> &_buffer) const override {
		_buffer.Append(HTTPGetVersionString<CharT>(this->m_version));
		_buffer.Append(HTTPGetNewlineString<CharT>());

		_state = State::HeaderTag;
	}

	public:
		HTTPRequest() = default;
		~HTTPRequest() = default;

		void Parse(const HTTPString<CharT> &_buffer) override {
			HTTPHeader<CharT> header;
			State state = State::Method;
			size_t index = 0;

			Clear();

			while (state != State::Idle) {
				switch (state) {
					case State::Method:
					{
						m_method = ParseMethod(state, index, _buffer);
						break;
					}
					case State::URI:
					{
						m_uri = ParseURI(state, index, _buffer);
						break;
					}
					case State::Version:
					{
						this->m_version = this->ParseVersion(state, index, _buffer);
						break;
					}
					case State::HeaderTag:
					{
						header.SetTag(this->ParseHeaderTag(state, index, _buffer));
						break;
					}
					case State::HeaderValue:
					{
						header.SetValue(this->ParseHeaderValue(state, index, _buffer));
						this->m_headers.Add(header);
						break;
					}
					case State::Message:
					{
						this->m_message = this->ParseMessage(state, index, _buffer, this->UseChunkedTransferEncoding());
						break;
					}
					case State::Idle:
					{
						break;
					}
					default:
					{
						throw std::runtime_error("Error in parsing.");
						break;
					}
				};
			}
		}

		void Serialize(HTTPString<CharT> &_buffer) const override {
			State state = State::Method;

			while (state != State::Idle) {
				switch (state) {
				case State::Method:
				{
					SerializeMethod(state, _buffer);
					break;
				}
				case State::URI:
				{
					SerializeURI(state, _buffer);
					break;
				}
				case State::Version:
				{
					this->SerializeVersion(state, _buffer);
					break;
				}
				case State::HeaderTag:
				{
					this->SerializeHeaders(state, _buffer);
					break;
				}
				case State::Message:
				{
					this->SerializeMessage(state, _buffer, this->UseChunkedTransferEncoding());
				}
				case State::Idle:
				{
					break;
				}
				default:
				{
					throw std::runtime_error("Error in serialization.");
					break;
				}
				};
			}
		}

		void Clear() override {
			this->m_method = {};
			this->m_uri = {};
			this->m_version = HTTPVersion::Unknown;
			this->m_headers.Clear();
			this->m_message = {};
		}
	};
}
