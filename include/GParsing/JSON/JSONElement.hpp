#pragma once
#include "GParsing/Core/API.hpp"

namespace GParsing {
  template <typename CharT>
	GPARSING_API class JSONElement
	{
	public:
		virtual bool Parse(const CharT* const _buffer, const size_t _bufferLen) = 0;
		virtual bool Parse(const std::vector<CharT>& _buffer) = 0;

		virtual bool Serialize(std::vector<CharT>& _buffer) = 0;

		virtual JSONElement* Copy() const = 0;
	};
}