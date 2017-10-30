#include "../common/header.h"

#include "Weave.h"

namespace Omni {
	ExceptionUnhandledError::ExceptionUnhandledError(ErrorCode::error_code ec, std::string && location)
		: ec(ec), location(location), w("Un-handled Error: " + ec.message() + " " + location) {}
	ExceptionUnhandledError::~ExceptionUnhandledError() {}

	const char* ExceptionUnhandledError::what() const noexcept {
		return w.c_str();
	}
}
