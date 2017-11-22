#pragma once

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef _WIN32
#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#ifdef __linux__
#elif _WIN32
// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#define USE_WIDECHAR_API 1
#else
#endif

#if defined(_MSC_VER)
    #define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
#else
    #pragma warning Unknown dynamic link import/export semantics.
#endif
