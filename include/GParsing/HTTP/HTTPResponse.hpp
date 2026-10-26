#pragma once
#include "GParsing/HTTP/HTTPBaseStructure.hpp"
#include "GParsing/HTTP/HTTPWhitespace.hpp"

namespace GParsing {
	template<typename CharT>
	class HTTPResponse : public HTTPBaseStructure<CharT> {
	private:
		using typename HTTPBaseStructure<CharT>::State;
		HTTPString<CharT> m_responseCode, m_responseMessage;

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

					if (HTTPIsNewline(_index, _buffer)) { 
						_state = State::Error; 
						return HTTPVersion::Unknown;
					}

					_state = State::ResponseCode;
					return HTTPGetVersion(version);
				}

				_index++;
			}

			_state = State::Error;
			return HTTPVersion::Unknown;
		}

		HTTPString<CharT> ParseResponseCode(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
			HTTPString<CharT> responseCode;

			this->SkipWhitespace(_index, _buffer);

			while (_index < _buffer.Length()) {
				if (!HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
					responseCode.Append(_buffer[_index]);
				}
				else {
					this->SkipWhitespace(_index, _buffer);

					if (HTTPIsNewline(_index, _buffer)) { 
						_index += HTTPGetNewlineSize(_index, _buffer);
						_state = State::HeaderTag; 
						return responseCode;
					}

					_state = State::ResponseMessage;
					return responseCode;
				}

				_index++;
			}

			_state = State::Error;
			return {};
		}

		HTTPString<CharT> ParseResponseMessage(State &_state, size_t &_index, const HTTPString<CharT> &_buffer) {
			HTTPString<CharT> responseMessage;

			this->SkipWhitespace(_index, _buffer);

			while (_index < _buffer.Length()) {
				if (!HTTPIsWhitespace(_index, _buffer) && !HTTPIsNewline(_index, _buffer)) {
					responseMessage.Append(_buffer[_index]);
				}
				else {
					this->SkipWhitespace(_index, _buffer);

					if (!HTTPIsNewline(_index, _buffer)) { 
						_state = State::Error; 
						return {};
					}
					else _index += HTTPGetNewlineSize(_index, _buffer);

					_state = State::HeaderTag;
					return responseMessage;
				}

				_index++;
			}

			_state = State::Error;
			return {};
		}

		void SerializeVersion(State &_state, HTTPString<CharT> &_buffer) const override {
			_buffer.Append(HTTPGetVersionString<CharT>(this->m_version));
			_buffer.Append(HTTPGetWhitespaceString<CharT>());

			_state = State::ResponseCode;
		}

		void SerializeResponseCode(State &_state, HTTPString<CharT> &_buffer) const {
			_buffer.Append(m_responseCode);
			_buffer.Append(HTTPGetWhitespaceString<CharT>());

			_state = State::ResponseMessage;
		}

		void SerializeResponseMessage(State &_state, HTTPString<CharT> &_buffer) const {
			_buffer.Append(m_responseMessage);
			_buffer.Append(HTTPGetNewlineString<CharT>());

			_state = State::HeaderTag;
		}

	public:
		void Parse(const HTTPString<CharT> &_buffer) override {
			HTTPHeader<CharT> header;
			State state = State::Version;
			size_t index = 0;

			Clear();

			while (state != State::Idle) {
				switch (state) {
					case State::Version:
					{
						this->m_version = this->ParseVersion(state, index, _buffer);
						break;
					}
					case State::ResponseCode:
					{
						this->m_responseCode = ParseResponseCode(state, index, _buffer);
						break;
					}
					case State::ResponseMessage:
					{
						this->m_responseMessage = ParseResponseMessage(state, index, _buffer);
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
			State state = State::Version;

			while (state != State::Idle) {
				switch (state) {
				case State::Version:
				{
					this->SerializeVersion(state, _buffer);
					break;
				}
				case State::ResponseCode:
				{
					SerializeResponseCode(state, _buffer);
					break;
				}
				case State::ResponseMessage:
				{
					SerializeResponseMessage(state, _buffer);
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
			this->m_version = HTTPVersion::Unknown;
			this->m_responseCode = {};
			this->m_responseMessage = {};
			this->m_headers.Clear();
			this->m_message = {};
		}
	};
}
