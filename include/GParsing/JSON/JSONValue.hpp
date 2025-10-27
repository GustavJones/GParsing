#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/JSON/JSONString.hpp"
#include "GParsing/JSON/JSONNumber.hpp"
#include "GParsing/JSON/JSONExpression.hpp"
#include <string>
#include <vector>

namespace GParsing {
	template<typename CharT>
	class JSONArray;

	template<typename CharT>
	class JSONObject;

  template <typename CharT>
	GPARSING_API class JSONValue
	{
	public:
		JSONValue() : m_type(Type::UNKNOWN), m_element(nullptr) {};
		JSONValue(JSONElement<CharT>* const _value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetValue(_value);
		}
		JSONValue(const JSONString<CharT> &_value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetString(_value);
		}
		JSONValue(const JSONNumber<CharT> &_value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetNumber(_value);
		}
		JSONValue(const JSONObject<CharT> &_value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetObject(_value);
		}
		JSONValue(const JSONArray<CharT> &_value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetArray(_value);
		}
		JSONValue(const JSONExpression<CharT> &_value) : m_type(Type::UNKNOWN), m_element(nullptr) {
			SetExpression(_value);
		}

		JSONValue(const JSONValue& _val) : m_type(_val.m_type), m_element(_val.m_element ? _val.m_element->Copy() : nullptr) {};
		JSONValue(JSONValue&& _val) : m_type(_val.m_type), m_element(_val.m_element) {
			_val.m_element = nullptr;
			_val.m_type = Type::UNKNOWN;
		};
		JSONValue& operator=(const JSONValue& _val) {
			if (&_val == this)
			{
				return *this;
			}

			delete m_element;
			m_element = _val.m_element ? _val.m_element->Copy() : nullptr;

			m_type = _val.m_type;

			return *this;
		}
		JSONValue& operator=(JSONValue&& _val) {
			if (&_val == this)
			{
				return *this;
			}

			delete m_element;
			m_element = _val.m_element;
			_val.m_element = nullptr;

			m_type = _val.m_type;
			_val.m_type = Type::UNKNOWN;

			return *this;
		}

		~JSONValue() { delete m_element; };

		bool Parse(const CharT* const _buffer, const size_t _bufferLen) {
			std::vector<CharT> buffer;

			buffer.resize(_bufferLen);
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

			JSONString<CharT> jsonString;
			JSONNumber<CharT> jsonNumber;
			JSONObject<CharT> jsonObject;
			JSONArray<CharT> jsonArray;
			JSONExpression<CharT> jsonExpression;

			if (jsonString.Parse(buffer))
			{
				delete m_element;
				m_element = new JSONString<CharT>(jsonString);
				m_type = Type::STRING;
			}
			else if (jsonNumber.Parse(buffer))
			{
				delete m_element;
				m_element = new JSONNumber<CharT>(jsonNumber);
				m_type = Type::NUMBER;
			}
			else if (jsonObject.Parse(buffer))
			{
				delete m_element;
				m_element = new JSONObject<CharT>(jsonObject);
				m_type = Type::OBJECT;
			}
			else if (jsonArray.Parse(buffer))
			{
				delete m_element;
				m_element = new JSONArray<CharT>(jsonArray);
				m_type = Type::ARRAY;
			}
			else if (jsonExpression.Parse(buffer))
			{
				delete m_element;
				m_element = new JSONExpression<CharT>(jsonExpression);
				m_type = Type::EXPRESSION;
			}
			else
			{
				return false;
			}

			return true;
		}

		bool Parse(const std::vector<CharT>& _buffer) {
			return Parse(_buffer.data(), _buffer.size());
		}

		bool Serialize(std::vector<CharT>& _buffer) {
			switch (m_type)
			{
			case GParsing::JSONValue<CharT>::Type::UNKNOWN:
			{
				_buffer.clear();
				return false;
				break;
			}
			case GParsing::JSONValue<CharT>::Type::STRING:
			{
				auto *element = dynamic_cast<JSONString<CharT> *>(m_element);

				if (!element)
				{
					_buffer.clear();
					return false;
				}
				else
				{
					if (!element->Serialize(_buffer))
					{
						_buffer.clear();
						return false;
					}
				}

				break;
			}
			case GParsing::JSONValue<CharT>::Type::NUMBER:
			{
				auto* element = dynamic_cast<JSONNumber<CharT> *>(m_element);

				if (!element)
				{
					_buffer.clear();
					return false;
				}
				else
				{
					if (!element->Serialize(_buffer))
					{
						_buffer.clear();
						return false;
					}
				}

				break;
			}
			case GParsing::JSONValue<CharT>::Type::OBJECT:
			{
				auto* element = dynamic_cast<JSONObject<CharT> *>(m_element);

				if (!element)
				{
					_buffer.clear();
					return false;
				}
				else
				{
					if (!element->Serialize(_buffer))
					{
						_buffer.clear();
						return false;
					}
				}

				break;
			}
			case GParsing::JSONValue<CharT>::Type::ARRAY:
			{
				auto* element = dynamic_cast<JSONArray<CharT> *>(m_element);

				if (!element)
				{
					_buffer.clear();
					return false;
				}
				else
				{
					if (!element->Serialize(_buffer))
					{
						_buffer.clear();
						return false;
					}
				}

				break;
			}
			case GParsing::JSONValue<CharT>::Type::EXPRESSION:
			{
				auto* element = dynamic_cast<JSONExpression<CharT> *>(m_element);

				if (!element)
				{
					_buffer.clear();
					return false;
				}
				else
				{
					if (!element->Serialize(_buffer))
					{
						_buffer.clear();
						return false;
					}
				}

				break;
			}
			}

			return true;
		}

		const JSONElement<CharT>* const GetValue() const { return m_element; }
		JSONElement<CharT>* const GetValue() { return m_element; }

		bool GetString(std::vector<CharT> &_str) {
			if (m_type != Type::STRING)
			{
				_str.clear();
				return false;
			}
			else
			{
				auto *string = dynamic_cast<JSONString<CharT> *>(m_element);
				_str = string->GetString();
				return true;
			}
		}

		bool GetString(JSONString<CharT> &_str) {
			if (m_type != Type::STRING)
			{
				_str = JSONString<CharT>();
				return false;
			}
			else
			{
				auto *string = dynamic_cast<JSONString<CharT> *>(m_element);
				_str = *string;
				return true;
			}
		}

		bool GetNumber(double_t& _num) {
			if (m_type != Type::NUMBER)
			{
				_num = 0;
				return false;
			}
			else
			{
				auto* number = dynamic_cast<JSONNumber<CharT> *>(m_element);
				_num = number->GetNumber();
				return true;
			}
		}

		bool GetNumber(JSONNumber<CharT>& _num) {
			if (m_type != Type::NUMBER)
			{
				_num = JSONNumber<CharT>();
				return false;
			}
			else
			{
				auto* number = dynamic_cast<JSONNumber<CharT> *>(m_element);
				_num = *number;
				return true;
			}
		}

		bool GetObject(JSONObject<CharT>& _obj) {
			if (m_type != Type::OBJECT)
			{
				_obj = JSONObject<CharT>();
				return false;
			}
			else
			{
				auto* object = dynamic_cast<JSONObject<CharT> *>(m_element);
				_obj = *object;
				return true;
			}
		}

		bool GetArray(JSONArray<CharT>& _arr) {
			if (m_type != Type::ARRAY)
			{
				_arr = JSONArray<CharT>();
				return false;
			}
			else
			{
				auto* array = dynamic_cast<JSONArray<CharT> *>(m_element);
				_arr = *array;
				return true;
			}
		}

		bool GetExpression(JSONExpression<CharT> &_exp) {
			if (m_type != Type::EXPRESSION)
			{
				_exp = JSONExpression<CharT>();
				return false;
			}
			else
			{
				auto* expression = dynamic_cast<JSONExpression<CharT> *>(m_element);
				_exp = *expression;
				return true;
			}
		}

		void SetValue(JSONElement<CharT>* const _value) {
			JSONString<CharT>* string = dynamic_cast<JSONString<CharT>*>(_value);
			JSONNumber<CharT>* number = dynamic_cast<JSONNumber<CharT>*>(_value);
			JSONObject<CharT>* object = dynamic_cast<JSONObject<CharT>*>(_value);
			JSONArray<CharT>* array = dynamic_cast<JSONArray<CharT>*>(_value);
			JSONExpression<CharT>* expression = dynamic_cast<JSONExpression<CharT>*>(_value);

			if (string)
			{
				m_type = Type::STRING;
			}
			else if (number)
			{
				m_type = Type::NUMBER;
			}
			else if (object)
			{
				m_type = Type::OBJECT;
			}
			else if (array)
			{
				m_type = Type::ARRAY;
			}
			else if (expression)
			{
				m_type = Type::EXPRESSION;
			}
			else
			{
				m_type = Type::UNKNOWN;
			}

			delete m_element;
			m_element = _value;
		}

		void SetString(const JSONString<CharT>& _value) {
			SetValue(new JSONString<CharT>(_value));
		}

		void SetString(const std::vector<CharT>& _str) {
			JSONString<CharT>* string = new JSONString<CharT>();
			string->SetString(_str);
			SetValue(string);
		}

		void SetString(const std::string & _str) {
			JSONString<CharT>* string = new JSONString<CharT>();
			string->SetString(_str);
			SetValue(string);
		}

		void SetNumber(const JSONNumber<CharT> &_value) {
			SetValue(new JSONNumber<CharT>(_value));
		}

		void SetNumber(const double_t& _value) {
			JSONNumber<CharT>* number = new JSONNumber<CharT>();
			number->SetNumber(_value);
			SetValue(number);
		}

		void SetObject(const JSONObject<CharT>& _value) {
			SetValue(new JSONObject<CharT>(_value));
		}

		void SetArray(const JSONArray<CharT>& _value) {
			SetValue(new JSONArray<CharT>(_value));
		}

		void SetExpression(const JSONExpression<CharT>& _value) {
			SetValue(new JSONExpression<CharT>(_value));
		}

	private:
		enum class Type
		{
			UNKNOWN,
			STRING,
			NUMBER,
			OBJECT,
			ARRAY,
			EXPRESSION
		} m_type;

		JSONElement<CharT>* m_element;

	public:
		Type GetType() const { return m_type; }
	};
}