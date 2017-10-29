#pragma once

#include "shared.h"

#include <exception>
#include <string>
#include <boost/preprocessor/stringize.hpp>

namespace Omni {
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4275)
#endif
	class SHARED Exception : public std::exception {
	public:
		explicit Exception();
		virtual ~Exception() = 0;
	};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

	class SHARED ExceptionSimpleString : public Exception {
	public:
		explicit ExceptionSimpleString(const std::string && what);
		virtual ~ExceptionSimpleString();

		virtual const char* what() const noexcept;
	private:
#pragma warning(push)
#pragma warning(disable: 4251)
		const std::string w;
#pragma warning(pop)

	};

	class SHARED ExceptionInternalError : public ExceptionSimpleString {
	public:
		ExceptionInternalError(const char *msg);
		virtual ~ExceptionInternalError();
	};

#define OMNI_INTERNAL_ERROR { throw ::Omni::ExceptionInternalError("Internal error. (@" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__) ")"); }

	class SHARED ExceptionFactoryNotFound : public ExceptionSimpleString {
	public:
		ExceptionFactoryNotFound(const std::string & name);
		virtual ~ExceptionFactoryNotFound();
	};

	class SHARED ExceptionModuleNotFound : public ExceptionSimpleString {
	public:
		ExceptionModuleNotFound(const std::string & name, const std::string & reason);
		virtual ~ExceptionModuleNotFound();
	};

	class SHARED ExceptionInvalidArgument : public ExceptionSimpleString {
	public:
		ExceptionInvalidArgument(const std::string & reason);
		virtual ~ExceptionInvalidArgument();
	};
}
