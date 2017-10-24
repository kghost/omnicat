#pragma once

#include <exception>
#include <string>
#include <boost/preprocessor/stringize.hpp>

namespace Omni {
	class EXPORT Exception : public std::exception {
	public:
		Exception();
		Exception(const std::string && what);
		~Exception();

		virtual const char* what() const noexcept;
	private:
		const std::string w;
	};

	class EXPORT ExceptionInternalError : public Exception {
	public:
		ExceptionInternalError(const char *msg);
		~ExceptionInternalError();
	};

#define OMNI_INTERNAL_ERROR { throw ::Omni::ExceptionInternalError("Internal error. (@" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__) ")"); }

	class EXPORT ExceptionClassNotFound : public Exception {
	public:
		ExceptionClassNotFound(const std::string & name);
		~ExceptionClassNotFound();
	};

	class EXPORT ExceptionModuleNotFound : public Exception {
	public:
		ExceptionModuleNotFound(const std::string & name, const std::string & reason);
		~ExceptionModuleNotFound();
	};
}
