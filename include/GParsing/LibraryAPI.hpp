#pragma once

#ifdef GPARSING_WINDOWS
#ifdef GPARSING_BUILD_DLL
#define GPARSING_API __declspec(dllexport)
#else
#define GPARSING_API __declspec(dllimport)
#endif
#else
#define GPARSING_API
#endif
