#pragma once

#include <codecvt>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/ref.hpp>

#include "shared.h"

namespace Omni {
#ifdef USE_WIDECHAR_API
	template<
		class Elem,
		unsigned long Maxcode = 0x10ffff,
		std::codecvt_mode Mode = (std::codecvt_mode)0
	> class codecvt_utf8_utf16_wchar : public std::codecvt<Elem, wchar_t, std::mbstate_t> {
	private:
		std::codecvt_utf8_utf16<extern_type> impl;
	};

	extern SHARED_DEFINE boost::iostreams::stream<
		boost::iostreams::code_converter<std::wostream, codecvt_utf8_utf16_wchar<char, 0x10ffff, std::consume_header>>
	> utf8cout;
#endif
}
