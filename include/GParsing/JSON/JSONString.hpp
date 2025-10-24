#pragma once
#include "GParsing/JSON/JSONElement.hpp"

namespace GParsing {
	template <typename CharT>
	GPARSING_API class JSONString : public JSONElement<CharT>
	{
	public:
		const std::vector<CharT> &GetString() const { return m_string; }
		void SetString(const std::vector<CharT>&_string) { m_string = _string; }

		bool Parse(const CharT* const _buffer, const size_t _bufferLen) override {
			State state = State::NORMAL;

			size_t index = 0;

			while (index < _bufferLen)
			{
				switch (state)
				{
				case State::NORMAL:
				{
					switch (_buffer[index])
					{
					case '\"':
					{
						state = State::TEXT;
						break;
					}
					case static_cast<CharT>(JSONWhitespace::SPACE):
					{
						break;
					}
					case static_cast<CharT>(JSONWhitespace::LINEFEED):
					{
						break;
					}
					case static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB):
					{
						break;
					}
					case static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN):
					{
						break;
					}
					default:
						return false;
						break;
					}

					break;
				}
				case State::TEXT:
				{
					switch (_buffer[index])
					{
					case '\\':
					{
						state = State::ESCAPE;
						break;
					}
					case '\"':
					{
						index = _bufferLen;
						break;
					}
					default:
						m_string.push_back(_buffer[index]);
						break;
					}

					break;
				}
				case State::ESCAPE:
				{
					switch (_buffer[index])
					{
					case '\\':
					{
						state = State::NORMAL;
						m_string.push_back('\\');
						break;
					}
					case '\"':
					{
						state = State::NORMAL;
						m_string.push_back('\"');
						break;
					}
					case '/':
					{
						state = State::NORMAL;
						m_string.push_back('/');
						break;
					}
					case 'b':
					{
						state = State::NORMAL;
						m_string.push_back('\b');
						break;
					}
					case 'f':
					{
						state = State::NORMAL;
						m_string.push_back('\f');
						break;
					}
					case 'n':
					{
						state = State::NORMAL;
						m_string.push_back('\n');
						break;
					}
					case 'r':
					{
						state = State::NORMAL;
						m_string.push_back('\r');
						break;
					}
					case 't':
					{
						state = State::NORMAL;
						m_string.push_back('\t');
						break;
					}
					case 'u':
					{
						state = State::UNICODE;
						break;
					}
					default:
						m_string.clear();
						return false;
						break;
					}

					break;
				}
				case State::UNICODE:
				{
					std::string hex;

					if (index > _bufferLen - 4)
					{
						m_string.clear();
						return false;
					}

					for (size_t i = 0; i < 4; i++)
					{
						hex += _buffer[index + i];
					}

					try
					{
						m_string.push_back(std::stoull(hex, nullptr, 16));
					}
					catch (const std::exception&)
					{
						m_string.clear();
						return false;
					}

					state = State::NORMAL;
					index += 3;

					break;
				}
				}

				index++;
			}

			return true;
		}

		bool Parse(const std::vector<CharT>& _buffer) override {
			return Parse(_buffer.data(), _buffer.size());
		}

		bool Serialize(std::vector<CharT>& _buffer) override {
			_buffer = m_string;

			_buffer.insert(_buffer.begin(), '\"');
			_buffer.insert(_buffer.end(), '\"');
			return true;
		}

		void Append(const std::vector<CharT> &_append) {
			m_string.insert(m_string.end(), _append.begin(), _append.end());
		}

		void Append(const CharT _append) {
			m_string.push_back(_append);
		}

		JSONString* Copy() const override {
			return new JSONString(*this);
		}

	private:
		enum class State{
			NORMAL,
			TEXT,
			ESCAPE,
			UNICODE
		};

		std::vector<CharT> m_string;
	};
}