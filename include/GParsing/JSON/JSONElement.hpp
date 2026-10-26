#pragma once
#include <vector>
#include <cstddef>

namespace GParsing {
  template <typename CharT>
	class JSONElement
	{
	public:
		virtual ~JSONElement() = default;

		virtual bool Parse(const CharT* const _buffer, const size_t _bufferLen) = 0;
		virtual bool Parse(const std::vector<CharT>& _buffer) = 0;

		virtual bool Serialize(std::vector<CharT>& _buffer) = 0;

		virtual JSONElement* Copy() const = 0;
	};
}
