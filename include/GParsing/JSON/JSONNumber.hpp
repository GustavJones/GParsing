#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "GParsing/JSON/JSONElement.hpp"

namespace GParsing {
	template <typename CharT> 
	GPARSING_API class JSONNumber : public JSONElement<CharT>
	{
	public:
		double_t GetNumber() const { return m_number; }
		void SetNumber(double_t _num) { m_number = _num; }

		bool Parse(const CharT *const _buffer, const size_t _bufferLen) override {
			size_t index = 0;
			std::string numberString;

			bool isWhitespace = true;
			while (index < _bufferLen && isWhitespace)
			{
				switch (static_cast<JSONWhitespace>(_buffer[index]))
				{
				case JSONWhitespace::SPACE:
					index++;
					break;
				case JSONWhitespace::LINEFEED:
					index++;
					break;
				case JSONWhitespace::HORIZONTAL_TAB:
					index++;
					break;
				case JSONWhitespace::CARRIAGE_RETURN:
					index++;
					break;
				default:
					isWhitespace = false;
					break;
				}
			}

			while (index < _bufferLen && !isWhitespace)
			{
				switch (static_cast<JSONWhitespace>(_buffer[index]))
				{
				case JSONWhitespace::SPACE:
					isWhitespace = true;
					break;
				case JSONWhitespace::LINEFEED:
					isWhitespace = true;
					break;
				case JSONWhitespace::HORIZONTAL_TAB:
					isWhitespace = true;
					break;
				case JSONWhitespace::CARRIAGE_RETURN:
					isWhitespace = true;
					break;
				default:
					numberString += _buffer[index];
					break;
				}

				index++;
			}

			try
			{
				m_number = std::stod(numberString);
				return true;
			}
			catch (const std::exception&)
			{
				m_number = 0;
				return false;
			}
		}

		bool Parse(const std::vector<CharT> &_buffer) override {
			return Parse(_buffer.data(), _buffer.size());
		}

		bool Serialize(std::vector<CharT> &_buffer) override {
			std::string numberString;
			try
			{
				numberString = std::to_string(GetNumber());
			}
			catch (const std::exception&)
			{
				return false;
			}

			_buffer.resize(numberString.length());

			for (size_t i = 0; i < numberString.length(); i++)
			{
				_buffer[i] = numberString[i];
			}

			return true;
		}

		JSONNumber* Copy() const override {
			return new JSONNumber(*this);
		}

		operator double_t() const {
			return m_number;
		}
	private:
		double_t m_number;
	};
}