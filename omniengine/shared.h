#pragma once

#if defined(_MSC_VER)

#ifdef OMNIENGINE_EXPORTS
#define SHARED_DECLARE extern __declspec(dllexport)
#define SHARED_DEFINE __declspec(dllexport)
#else
#define SHARED_DECLARE extern __declspec(dllimport)
#define SHARED_DEFINE __declspec(dllimport)
#endif

#elif defined(__GNUC__)

#ifdef OMNIENGINE_EXPORTS
#define SHARED_CLASS __attribute__((visibility("default")))
#define SHARED_INSTANCE extern __attribute__((visibility("default")))
#else
#define SHARED_CLASS
#define SHARED_INSTANCE extern
#endif

#else
#pragma warning Unknown dynamic link import/export semantics.
#endif
