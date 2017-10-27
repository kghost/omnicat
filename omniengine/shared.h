#pragma once

#if defined(_MSC_VER)

#ifdef OMNIENGINE_EXPORTS
#define SHARED_MEMBER
#define SHARED __declspec(dllexport)
#else
#define SHARED_MEMBER
#define SHARED __declspec(dllimport)
#endif

#elif defined(__GNUC__)

#ifdef OMNIENGINE_EXPORTS
#define SHARED_MEMBER __attribute__((visibility("default")))
#define SHARED __attribute__((visibility("default")))
#else
#define SHARED_MEMBER
#define SHARED
#endif

#else
#pragma warning Unknown dynamic link import/export semantics.
#endif
