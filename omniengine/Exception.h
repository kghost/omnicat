#pragma once

#include "shared.h"

#include <exception>
#include <string>
#include <boost/preprocessor/stringize.hpp>

namespace Omni {
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif
	class SHARED_DEFINE Exception : public std::exception {
	public:
		Exception();
		Exception(const std::string && what);
		~Exception();

		virtual const char* what() const noexcept;
	private:
		const std::string w;
	};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

	class SHARED_DEFINE ExceptionInternalError : public Exception {
	public:
		ExceptionInternalError(const char *msg);
		~ExceptionInternalError();
	};

#define OMNI_INTERNAL_ERROR { throw ::Omni::ExceptionInternalError("Internal error. (@" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__) ")"); }

	class SHARED_DEFINE ExceptionClassNotFound : public Exception {
	public:
		ExceptionClassNotFound(const std::string & name);
		~ExceptionClassNotFound();
	};

	class SHARED_DEFINE ExceptionModuleNotFound : public Exception {
	public:
		ExceptionModuleNotFound(const std::string & name, const std::string & reason);
		~ExceptionModuleNotFound();
	};
}
