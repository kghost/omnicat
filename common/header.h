#pragma once

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#ifdef __linux__
#elif _WIN32
// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
static inline std::wstring convUTF8toUCS2(const std::string & s) { return converter.from_bytes(s); }
static inline std::string convUCS2toUTF8(const std::wstring & s) { return converter.to_bytes(s); }

#define USE_WIDECHAR_API 1
#else
#endif

#if defined(_MSC_VER)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif
