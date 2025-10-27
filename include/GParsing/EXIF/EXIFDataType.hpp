#pragma once
#include "GParsing/Core/API.hpp"
#include <unordered_map>

namespace GParsing {
	GPARSING_API enum class EXIFDataType : uint16_t
	{
		UNKNOWN = 0x00,
		UBYTE = 0x01,
		ASCII = 0x02,
		USHORT = 0x03,
		ULONG = 0x04,
		URATIONAL = 0x05,
		BYTE = 0x06,
		UNDEFINED = 0x07,
		SHORT = 0x08,
		LONG = 0x09,
		RATIONAL = 0x0A,
		FLOAT = 0x0B,
		DOUBLE = 0x0C
	};

	static const std::unordered_map<EXIFDataType, uint16_t> EXIFDataTypeSizes = {
		{EXIFDataType::UNKNOWN, 0},
		{EXIFDataType::UBYTE, 1},
		{EXIFDataType::ASCII, 1},
		{EXIFDataType::USHORT, 2},
		{EXIFDataType::ULONG, 4},
		{EXIFDataType::URATIONAL, 8},
		{EXIFDataType::BYTE, 1},
		{EXIFDataType::UNDEFINED, 1},
		{EXIFDataType::SHORT, 2},
		{EXIFDataType::LONG, 4},
		{EXIFDataType::RATIONAL, 8},
		{EXIFDataType::FLOAT, 4},
		{EXIFDataType::DOUBLE, 8},
	};
}