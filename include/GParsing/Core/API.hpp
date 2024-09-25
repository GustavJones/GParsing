#pragma once

#if defined(_MSC_VER)
#undef GPARSING_API

#ifdef GPARSING_SHARED

#ifdef GPARSING_BUILD_DLL
#define GPARSING_API __declspec(dllexport)
#else
#define GPARSING_API __declspec(dllimport)
#endif

#else
#define GPARSING_API
#endif

#elif defined(__GNUC__)
#undef GPARSING_API

#ifdef GPARSING_SHARED

#ifdef GPARSING_BUILD_DLL
#define GPARSING_API __attribute__((visibility("default")))
#else
#define GPARSING_API
#endif

#else
#define GPARSING_API
#endif

#else
#define GPARSING_API
#endif
