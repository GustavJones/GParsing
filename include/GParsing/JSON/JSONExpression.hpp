#pragma once
#include "GParsing/Core/API.hpp"
#include "GParsing/Core/Buffers.hpp"
#include "GParsing/Core/Compare.hpp"
#include "GParsing/JSON/JSONElement.hpp"
#include <vector>

namespace GParsing {
	enum class JSONExpressionType
	{
		Unknown = -2,
		Null = -1,
		False = 0,
		True = 1,
	};

	static const std::vector<std::pair<std::string, JSONExpressionType>> JSONExpressionDefinitions = { {"true", JSONExpressionType::True}, {"false", JSONExpressionType::False}, {"null", JSONExpressionType::Null} };

	template <typename CharT>
	GPARSING_API class JSONExpression : public JSONElement<CharT>
	{
	public:
		JSONExpression(const JSONExpressionType _type) : m_type(_type) {}
		JSONExpression() : m_type(JSONExpressionType::Unknown) {};
		JSONExpression(const JSONExpression<CharT>& _exp) = default;
		JSONExpression(JSONExpression<CharT>&& _exp) = default;

		JSONExpression& operator=(const JSONExpression<CharT>& _exp) = default;
		JSONExpression& operator=(JSONExpression<CharT>&& _exp) = default;

		bool Parse(const CharT* const _buffer, const size_t _bufferLen) override {
			std::vector<CharT> expression(_bufferLen);

			for (size_t i = 0; i < _bufferLen; i++)
			{
				expression[i] = _buffer[i];
			}

			GParsing::RemoveCharacterPadding(expression, {
				static_cast<CharT>(JSONWhitespace::SPACE),
				static_cast<CharT>(JSONWhitespace::LINEFEED),
				static_cast<CharT>(JSONWhitespace::HORIZONTAL_TAB),
				static_cast<CharT>(JSONWhitespace::CARRIAGE_RETURN),
				});

			for (size_t i = 0; i < JSONExpressionDefinitions.size(); i++)
			{
				if (JSONExpressionDefinitions[i].first.length() != expression.size())
				{
					continue;
				}

				if (!CaseUnsensitiveMatch<CharT>(reinterpret_cast<const CharT *>(JSONExpressionDefinitions[i].first.c_str()), expression.data(), expression.size()))
				{
					continue;
				}
				else
				{
					m_type = JSONExpressionDefinitions[i].second;
					return true;
				}
			}

			return false;
		}

		bool Parse(const std::vector<CharT>& _buffer) override {
			return Parse(_buffer.data(), _buffer.size());
		}

		bool Serialize(std::vector<CharT>& _buffer) override {
			for (size_t i = 0; i < JSONExpressionDefinitions.size(); i++)
			{
				if (m_type == JSONExpressionDefinitions[i].second)
				{
					_buffer.resize(JSONExpressionDefinitions[i].first.length());
					
					for (size_t j = 0; j < JSONExpressionDefinitions[i].first.length(); j++)
					{
						_buffer[j] = JSONExpressionDefinitions[i].first[j];
					}

					return true;
				}
			}

			_buffer.clear();
			return false;
		}

		JSONExpression* Copy() const override {
			return new JSONExpression(*this);
		}

		JSONExpressionType GetExpression() const
		{
			return m_type;
		}

		void SetExpression(const JSONExpressionType _expression) {
			m_type = _expression;
		}
	private:

		JSONExpressionType m_type;
		
	};
}