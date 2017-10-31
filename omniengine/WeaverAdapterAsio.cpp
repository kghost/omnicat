#include "../common/header.h"

#include "WeaverAdapterAsio.h"

namespace Omni {
	namespace Fiber {
		namespace Asio {
			ExceptionUnhandledError::ExceptionUnhandledError(ErrorCode::error_code ec)
				: ec(ec), w("Un-handled Error: " + toUTF8(reinterpret_cast<const wchar_t*>(ec.message().c_str()))) {}
			ExceptionUnhandledError::~ExceptionUnhandledError() {}

			const char* ExceptionUnhandledError::what() const noexcept {
				return w.c_str();
			}
		}
	}
}
