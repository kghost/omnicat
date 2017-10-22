#pragma once

#include <exception>
#include <string>
#include <boost/preprocessor/stringize.hpp>

namespace Omni {
	class Exception : public std::exception {
	public:
		Exception() : w("Unknown  exception") {}
		Exception(const std::string && what) : w(what) {}

		virtual const char* what() const noexcept { return w.c_str(); }
	private:
		const std::string w;
	};

	class ExceptionInternalError : public Exception {
	public:
		ExceptionInternalError(const char *msg) : Exception(msg) {}
	};

#define OMNI_INTERNAL_ERROR { throw ::Omni::ExceptionInternalError("Internal error" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__)); }

	class ExceptionClassNotFound : public Exception {
	public:
		ExceptionClassNotFound(const std::string & name) : Exception("Can't fint -class: " + name) {}
	};

	class ExceptionModuleNotFound : public Exception {
	public:
		ExceptionModuleNotFound(const std::string & name, const std::string & reason) : Exception("Can't fint -module(" + name + "): " + reason) {}
	};
}
