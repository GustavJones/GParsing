#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Buffers.hpp"
#include "GParsing/JSON/JSONElement.hpp"
#include "GParsing/JSON/JSONValue.hpp"
#include <vector>
#include <utility>

namespace GParsing {
  template <typename CharT>
	GPARSING_API class JSONObject : public JSONElement<CharT>
	{
	public:
		JSONObject* Copy() const override {
			return new JSONObject(*this);
		}

		bool Parse(const CharT* const _buffer, const size_t _bufferLen) override {
			size_t objectScopeCount = 0;
			size_t arrayScopeCount = 0;
			State state;
			m_members.clear();

			std::vector<CharT> buffer(_bufferLen);
			std::vector<CharT> tempBuffer;
			std::pair<JSONString<CharT>, JSONValue<CharT>> member;
			state = State::NORMAL;
			size_t index = 0;

			for (size_t i = 0; i < _bufferLen; i++)
			{
				buffer[i] = _buffer[i];
			}

			RemoveCharacterPadding(buffer, {
				static_cast<CharT>(JSONWhitespace::SPACE),
				static_cast<CharT>(JSONWhitespace::LINEFEED),
				static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
				static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
				});

			if (buffer.size() <= 0)
			{
				m_members.clear();
				return false;
			}
			else if (buffer[0] != '{' || buffer[buffer.size() - 1] != '}')
			{
				m_members.clear();
				return false;
			}
			else
			{
				buffer.erase(buffer.begin(), buffer.begin() + 1);
				buffer.erase(buffer.end() - 1, buffer.end());
			}

			while (index < buffer.size())
			{
				switch (state)
				{
				case State::NORMAL:
				{
					switch (buffer[index])
					{
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
						state = State::KEY;
						tempBuffer.push_back(buffer[index]);
						break;
					}

					break;
				}
				case State::KEY:
				{
					switch (buffer[index])
					{
					case ':':
					{
						state = State::VALUE;

						if (!member.first.Parse(tempBuffer))
						{
							m_members.clear();
							return false;
						}
						tempBuffer.clear();

						break;
					}
					default:
						tempBuffer.push_back(buffer[index]);
						break;
					}

					break;
				}
				case State::VALUE:
				{
					switch (buffer[index])
					{
					case ',':
					{
						if (objectScopeCount > 0 || arrayScopeCount > 0)
						{
							tempBuffer.push_back(buffer[index]);
							break;
						}

						state = State::NORMAL;
						if (!member.second.Parse(tempBuffer))
						{
							m_members.clear();
							return false;
						}
						tempBuffer.clear();

						m_members.push_back(member);

						member = std::pair<JSONString<CharT>, JSONValue<CharT>>();

						break;
					}
					case '{':
					{
						objectScopeCount++;
						tempBuffer.push_back(buffer[index]);
						break;
					}
					case '[':
					{
						arrayScopeCount++;
						tempBuffer.push_back(buffer[index]);
						break;
					}
					case '}':
					{
						if (objectScopeCount > 0)
						{
							objectScopeCount--;
							tempBuffer.push_back(buffer[index]);
						}
						else
						{
							m_members.clear();
							return false;
						}

						break;
					}
					case ']':
					{
						if (arrayScopeCount > 0)
						{
							arrayScopeCount--;
							tempBuffer.push_back(buffer[index]);
						}
						else
						{
							m_members.clear();
							return false;
						}

						break;
					}
					default:
						tempBuffer.push_back(buffer[index]);

						if (index == buffer.size() - 1)
						{
							if (objectScopeCount > 0 || arrayScopeCount > 0)
							{
								m_members.clear();
								return false;
								break;
							}

							if (!member.second.Parse(tempBuffer))
							{
								m_members.clear();
								return false;
							}
							tempBuffer.clear();

							m_members.push_back(member);
						}
						break;
					}

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
			std::vector<CharT> tempBuffer;

			_buffer.clear();
			_buffer.push_back('{');

			for (size_t i = 0; i < m_members.size(); i++)
			{
				_buffer.push_back(' ');
				if (!m_members[i].first.Serialize(tempBuffer))
				{
					_buffer.clear();
					return false;
				}

				_buffer.insert(_buffer.end(), tempBuffer.begin(), tempBuffer.end());

				_buffer.push_back(':');
				_buffer.push_back(' ');

				if (!m_members[i].second.Serialize(tempBuffer))
				{
					_buffer.clear();
					return false;
				}

				_buffer.insert(_buffer.end(), tempBuffer.begin(), tempBuffer.end());

				if (i < m_members.size() - 1)
				{
					_buffer.push_back(',');
				}
			}

			_buffer.push_back(' ');
			_buffer.push_back('}');

			return true;
		}

	private:
		enum class State
		{
			NORMAL,
			KEY,
			VALUE,
		};

		std::vector<std::pair<JSONString<CharT>, JSONValue<CharT>>> m_members;
	};
}