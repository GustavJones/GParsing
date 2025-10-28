#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Buffers.hpp"
#include "GParsing/JSON/JSONWhitespace.hpp"
#include "GParsing/JSON/JSONValue.hpp"
#include <vector>

namespace GParsing {
  template <typename CharT>
	GPARSING_API class JSONArray : public JSONElement<CharT>
	{
	public:
		JSONArray() = default;

		JSONArray(const std::vector<JSONValue<CharT>> &_array) : m_values(_array) {}

		JSONArray* Copy() const override {
			return new JSONArray(*this);
		}

		bool Parse(const CharT* const _buffer, const size_t _bufferLen) override {
			State state;
			size_t objectScopeCount = 0;
			size_t arrayScopeCount = 0;
			size_t index = 0;
			std::vector<CharT> buffer(_bufferLen);
			std::vector<CharT> valueBuffer;
			state = State::NORMAL;

			for (size_t i = 0; i < _bufferLen; i++)
			{
				buffer[i] = _buffer[i];
			}

			GParsing::RemoveCharacterPadding(buffer, {
				static_cast<CharT>(JSONWhitespace::SPACE),
				static_cast<CharT>(JSONWhitespace::LINEFEED),
				static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
				static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
				});

			if (buffer.size() <= 0)
			{
				m_values.clear();
				return false;
			}
			else if (buffer[0] != '[' || buffer[buffer.size() - 1] != ']')
			{
				m_values.clear();
				return false;
			}
			else
			{
				buffer.erase(buffer.begin(), buffer.begin() + 1);
				buffer.erase(buffer.end() - 1, buffer.end());
			}

			if (buffer.size() == 0)
			{
				return true;
			}

			while (index < buffer.size())
			{
				switch (state)
				{
				case State::NORMAL:
				{
					switch (static_cast<JSONWhitespace>(buffer[index]))
					{
					case JSONWhitespace::SPACE:
					{
						break;
					}
					case JSONWhitespace::LINEFEED:
					{
						break;
					}
					case JSONWhitespace::HORIZONTAL_TAB:
					{
						break;
					}
					case JSONWhitespace::CARRIAGE_RETURN:
					{
						break;
					}
					default:
						state = State::VALUE;

						if (buffer[index] == '[')
						{
							arrayScopeCount++;
						}
						else if (buffer[index] == '{')
						{
							objectScopeCount++;
						}
						
						valueBuffer.push_back(buffer[index]);
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
						state = State::NORMAL;

						GParsing::RemoveCharacterPadding(valueBuffer, {
							static_cast<CharT>(JSONWhitespace::SPACE),
							static_cast<CharT>(JSONWhitespace::LINEFEED),
							static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
							static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
							});

						m_values.emplace_back();
						if (!m_values[m_values.size() - 1].Parse(valueBuffer)) {
							m_values.clear();
							return false;
						}

						valueBuffer.clear();
						break;
					}
					case '[':
					{
						arrayScopeCount++;
						valueBuffer.push_back(buffer[index]);
						break;
					}
					case '{':
					{
						objectScopeCount++;
						valueBuffer.push_back(buffer[index]);
						break;
					}
					case ']':
					{
						if (arrayScopeCount > 0)
						{
							arrayScopeCount--;
							valueBuffer.push_back(buffer[index]);
						}
						else
						{
							GParsing::RemoveCharacterPadding(valueBuffer, {
								static_cast<CharT>(JSONWhitespace::SPACE),
								static_cast<CharT>(JSONWhitespace::LINEFEED),
								static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
								static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
								});

							m_values.emplace_back();
							if (!m_values[m_values.size() - 1].Parse(valueBuffer))
							{
								m_values.clear();
								return false;
							}

							return true;
						}

						break;
					}
					case '}':
					{
						if (objectScopeCount > 0)
						{
							objectScopeCount--;
							valueBuffer.push_back(buffer[index]);
						}
						else
						{
							m_values.clear();
							return false;
						}

						break;
					}
					default:
						valueBuffer.push_back(buffer[index]);

						if (index == buffer.size() - 1)
						{
							GParsing::RemoveCharacterPadding(valueBuffer, {
								static_cast<CharT>(JSONWhitespace::SPACE),
								static_cast<CharT>(JSONWhitespace::LINEFEED),
								static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
								static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
								});

							m_values.emplace_back();
							if (!m_values[m_values.size() - 1].Parse(valueBuffer))
							{
								m_values.clear();
								return false;
							}

							return true;
						}
						break;
					}

					break;
				}
				}

				index++;
			}

			GParsing::RemoveCharacterPadding(valueBuffer, {
				static_cast<CharT>(JSONWhitespace::SPACE),
				static_cast<CharT>(JSONWhitespace::LINEFEED),
				static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
				static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
				});

			m_values.emplace_back();
			if (!m_values[m_values.size() - 1].Parse(valueBuffer))
			{
				m_values.clear();
				return false;
			}

			return true;
		}

		bool Parse(const std::vector<CharT>& _buffer) override {
			return Parse(_buffer.data(), _buffer.size());
		}

		bool Serialize(std::vector<CharT>& _buffer) override {
			std::vector<CharT> tempBuffer;

			_buffer.clear();
			_buffer.push_back('[');

			for (size_t i = 0; i < m_values.size(); i++)
			{
				if (!m_values[i].Serialize(tempBuffer))
				{
					_buffer.clear();
					return false;
				}

				_buffer.insert(_buffer.end(), tempBuffer.begin(), tempBuffer.end());
				
				if (i < m_values.size() - 1)
				{
					_buffer.push_back(',');
					_buffer.push_back(static_cast<CharT>(JSONWhitespace::SPACE));
				}
			}

			_buffer.push_back(']');

			return true;
		}

		const JSONValue<CharT>& operator[](const size_t _index) const {
			return GetValue(_index);
		}

		JSONValue<CharT>& operator[](const size_t _index) {
			return GetValue(_index);
		}

		const size_t GetSize() const {
			return m_values.size();
		}

		const JSONValue<CharT>& GetValue(const size_t _index) const {
			return m_values[_index];
		}

		JSONValue<CharT>& GetValue(const size_t _index) {
			return m_values[_index];
		}

		void PushValue(const JSONValue<CharT>& _value) {
			m_values.push_back(_value);
		}

		void PopValue()
		{
			m_values.pop_back();
		}

		void RemoveValue(const size_t _index) {
			m_values.erase(m_values.begin() + _index);
		}

		void Clear() {
			m_values.clear();
		}

	private:
		std::vector<JSONValue<CharT>> m_values;

		enum class State
		{
			NORMAL,
			VALUE,
		} m_state;
	};
}