#include "../common/header.h"

#include "WeaverAdapterAsio.h"

#include "encoding.h"

namespace Omni {
	namespace Fiber {
		namespace Asio {
			ExceptionUnhandledError::ExceptionUnhandledError(ErrorCode::error_code ec)
				: ec(ec), w("Un-handled Error: " + toUTF8(ec.message())) {}
			ExceptionUnhandledError::~ExceptionUnhandledError() {}

			const char* ExceptionUnhandledError::what() const noexcept {
				return w.c_str();
			}
		}
	}
}
