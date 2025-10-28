#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Buffers.hpp"
#include "GParsing/JSON/JSONElement.hpp"
#include "GParsing/JSON/JSONValue.hpp"
#include <vector>
#include <utility>
#include <stdexcept>
#include <fstream>

namespace GParsing {
  template <typename CharT>
	GPARSING_API class JSONObject : public JSONElement<CharT>
	{
	public:
		JSONObject() = default;
		JSONObject(const std::vector<JSONString<CharT>>& _keys, const std::vector<JSONValue<CharT>>& _values) {
			if (_keys.size() != _values.size())
			{
				throw std::runtime_error("Buffers not the same size");
			}

			m_members.reserve(_keys.size());
			for (size_t i = 0; i < _keys.size(); i++)
			{
				m_members.push_back({_keys[i], _values[i]});
			}
		}

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

		bool Parse(const std::string &_filePath) {
			std::fstream f;
			std::vector<CharT> buffer;

			f.open(_filePath, std::fstream::in | std::fstream::ate);
			if (f.is_open())
			{
				buffer.resize(f.tellg());
				f.seekg(f.beg);
				f.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
				f.close();
				return Parse(buffer);
			}
			else
			{
				f.close();
				return false;
			}
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

		bool Serialize(const std::string& _filePath) {
			std::fstream f;
			std::vector<CharT> buffer;

			if (!Serialize(buffer))
			{
				return false;
			}

			f.open(_filePath, std::fstream::out);
			if (f.is_open())
			{
				f.write(reinterpret_cast<char *>(buffer.data()), buffer.size());
				f.close();
				return true;
			}
			else
			{
				f.close();
				return false;
			}
		}

		void AddMember(const JSONString<CharT>& _key, const JSONValue<CharT>& _value) {
			m_members.push_back({ _key, _value });
		}

		void RemoveMember(const size_t _index) {
			m_members.erase(m_members.begin() + _index);
		}

		const size_t GetMembersCount() const {
			return m_members.size();
		}

		void GetMember(JSONString<CharT>& _key, JSONValue<CharT>& _value, const size_t _index) const {
			_key = m_members[_index].first;
			_value = m_members[_index].second;
		}

		void SetMember(const JSONString<CharT>& _key, const JSONValue<CharT>& _value, const size_t _index) {
			m_members[_index].first = _key;
			m_members[_index].second = _value;
		}

		JSONValue<CharT>& operator[](const JSONString<CharT> &_key) {
			for (auto& member : m_members)
			{
				if (member.first == _key)
				{
					return member.second;
				}
			}

			throw std::runtime_error("Key not in object");
		}

		const JSONValue<CharT>& operator[](const JSONString<CharT> &_key) const {
			for (auto& member : m_members)
			{
				if (member.first == _key)
				{
					return member.second;
				}
			}

			throw std::runtime_error("Key not in object");
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
