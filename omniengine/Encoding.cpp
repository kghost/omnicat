#include "../common/header.h"

#include "encoding.h"

#include <string>
#include <locale>
#include <memory>
#include <boost/locale/info.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/generator.hpp>

namespace Omni {
#ifdef _WIN32
    std::string fromSystemToU8(const std::wstring& wstr)
    {
        int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), nullptr, 0, nullptr, nullptr);
        std::string str(count, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), &str[0], count, nullptr, nullptr);
        return str;
    }

    std::wstring fromU8ToSystem(const std::string& str)
    {
        int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), wstr.data(), count);
        return wstr;
    }
#endif
}