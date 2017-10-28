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
	class SHARED Exception : public std::exception {
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

	class SHARED ExceptionInternalError : public Exception {
	public:
		ExceptionInternalError(const char *msg);
		~ExceptionInternalError();
	};

#define OMNI_INTERNAL_ERROR { throw ::Omni::ExceptionInternalError("Internal error. (@" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__) ")"); }

	class SHARED ExceptionFactoryNotFound : public Exception {
	public:
		ExceptionFactoryNotFound(const std::string & name);
		~ExceptionFactoryNotFound();
	};

	class SHARED ExceptionModuleNotFound : public Exception {
	public:
		ExceptionModuleNotFound(const std::string & name, const std::string & reason);
		~ExceptionModuleNotFound();
	};

	class SHARED ExceptionInvalidArgument : public Exception {
	public:
		ExceptionInvalidArgument(const std::string & reason);
		~ExceptionInvalidArgument();
	};

}
