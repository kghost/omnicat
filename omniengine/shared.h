#pragma once

#if defined(_MSC_VER)

#ifdef OMNIENGINE_EXPORTS
#define SHARED __declspec(dllexport)
#else
#define SHARED __declspec(dllimport)
#endif

#elif defined(__GNUC__)

#ifdef OMNIENGINE_EXPORTS
#define SHARED __attribute__((visibility("default")))
#else
#define SHARED
#endif

#else
#pragma warning Unknown dynamic link import/export semantics.
#endif
