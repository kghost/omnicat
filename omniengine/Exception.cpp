#include "../common/header.h"

#include "Exception.h"

namespace Omni {
	Exception::Exception() : w("Unknown  exception") {}
	Exception::Exception(const std::string && what) : w(what) {}
	Exception::~Exception() {}
	const char* Exception::what() const noexcept { return w.c_str(); }

	ExceptionInternalError::ExceptionInternalError(const char *msg) : Exception(msg) {}
	ExceptionInternalError::~ExceptionInternalError() {}

	ExceptionFactoryNotFound::ExceptionFactoryNotFound(const std::string & name) : Exception("Can't find Factory: " + name) {}
	ExceptionFactoryNotFound::~ExceptionFactoryNotFound() {}

	ExceptionModuleNotFound::ExceptionModuleNotFound(const std::string & name, const std::string & reason) : Exception("Can't find module(" + name + "): " + reason) {}
	ExceptionModuleNotFound::~ExceptionModuleNotFound() {}

	ExceptionInvalidArgument::ExceptionInvalidArgument(const std::string & reason) : Exception("Invlid argument: " + reason) {}
	ExceptionInvalidArgument::~ExceptionInvalidArgument() {}
}
