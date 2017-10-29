#include "../common/header.h"

#include "Exception.h"

namespace Omni {
	Exception::Exception() {}
	Exception::~Exception() {}

	ExceptionSimpleString::ExceptionSimpleString(const std::string && what) : w(what) {}
	ExceptionSimpleString::~ExceptionSimpleString() {}
	const char* ExceptionSimpleString::what() const noexcept { return w.c_str(); }

	ExceptionInternalError::ExceptionInternalError(const char *msg) : ExceptionSimpleString(msg) {}
	ExceptionInternalError::~ExceptionInternalError() {}

	ExceptionFactoryNotFound::ExceptionFactoryNotFound(const std::string & name) : ExceptionSimpleString("Can't find Factory: " + name) {}
	ExceptionFactoryNotFound::~ExceptionFactoryNotFound() {}

	ExceptionModuleNotFound::ExceptionModuleNotFound(const std::string & name, const std::string & reason) : ExceptionSimpleString("Can't find module(" + name + "): " + reason) {}
	ExceptionModuleNotFound::~ExceptionModuleNotFound() {}

	ExceptionInvalidArgument::ExceptionInvalidArgument(const std::string & reason) : ExceptionSimpleString("Invlid argument: " + reason) {}
	ExceptionInvalidArgument::~ExceptionInvalidArgument() {}
}
