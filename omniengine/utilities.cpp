#include "../common/header.h"

#include "utilities.h"

namespace Omni {
	SHARED_DEFINE boost::iostreams::stream<
		boost::iostreams::code_converter<std::wostream, codecvt_utf8_utf16_wchar<char, 0x10ffff, std::consume_header>>
	> utf8cout(boost::ref(std::wcout));
}
