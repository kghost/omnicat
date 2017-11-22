#include "../common/header.h"

#include "encoding.h"

#include <string>
#include <codecvt>
#include <locale>
#include <memory>
#include <boost/locale/info.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/generator.hpp>

namespace Omni {
	std::locale SystemLocale() {
		boost::locale::generator gen;
		return gen("");
	}

	static std::locale locale = SystemLocale();

	bool SystemIsUtf8() {
		auto& info = std::use_facet<boost::locale::info>(locale);
		return info.utf8();
	}

	static bool utf8 = SystemIsUtf8();

	SHARED std::string toUTF8(const std::string && s) {
		if (utf8) return std::move(s);
		return boost::locale::conv::to_utf<char>(s, locale);
	}

	static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	SHARED std::wstring fromUTF8(std::string && s) { return converter.from_bytes(std::move(s)); }
	SHARED std::wstring fromUTF8(const std::string & s) { return converter.from_bytes(s); }
	SHARED std::string toUTF8(std::wstring && s) { return converter.to_bytes(std::move(s)); }
	SHARED std::string toUTF8(const std::wstring & s) { return converter.to_bytes(s); }
}