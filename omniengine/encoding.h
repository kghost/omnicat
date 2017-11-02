#pragma once

#include <string>

#include "shared.h"

namespace Omni {
	extern SHARED std::string toUTF8(const std::string && s);

	extern SHARED std::wstring fromUTF8(std::string && s);
	extern SHARED std::wstring fromUTF8(const std::string & s);
	extern SHARED std::string toUTF8(std::wstring && s);
	extern SHARED std::string toUTF8(const std::wstring & s);
}
