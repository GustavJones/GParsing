#pragma once
#include "GParsing/Core/API.hpp"

namespace GParsing {
	GPARSING_API enum class JSONWhitespace
	{
		SPACE = ' ',
		LINEFEED = '\n',
		CARRIAGE_RETURN = '\r',
		HORIZONTAL_TAB = '\t'
	};
}