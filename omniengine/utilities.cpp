#include "../common/header.h"

#include "utilities.h"

namespace Omni {
#ifdef USE_WIDECHAR_API
	SHARED boost::iostreams::stream<
		code_converter_flushable<std::wistream, codecvt_utf16>
	> utf8cin(boost::ref(std::wcin));

	SHARED boost::iostreams::stream<
		code_converter_flushable<std::wostream, codecvt_utf16>
	> utf8cout(boost::ref(std::wcout));

	SHARED boost::iostreams::stream<
		code_converter_flushable<std::wostream, codecvt_utf16>
	> utf8cerr(boost::ref(std::wcerr));
#endif
}
