#pragma once

#include <string>

#include "shared.h"

namespace Omni {
#ifdef _WIN32
    SHARED std::string fromSystemToU8(const std::wstring& wstr);
    SHARED std::wstring fromU8ToSystem(const std::string& str);
#endif
}