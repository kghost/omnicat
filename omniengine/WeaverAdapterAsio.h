#pragma once

#include <boost/core/noncopyable.hpp>

#include "Weave.h"

namespace Omni {
	class SHARED ExceptionUnhandledError : public Exception {
	public:
		ExceptionUnhandledError(ErrorCode::error_code ec, std::string && location);
		virtual ~ExceptionUnhandledError();

		virtual const char* what() const noexcept;
	private:
#pragma warning(push)
#pragma warning(disable: 4251)
		ErrorCode::error_code ec;
		std::string location;
		std::string w;
#pragma warning(pop)
	};

	template<typename IoObject, typename R (IoObject::Fun)(... Args)>
	Fiber async(IoObject& o, Fun f) {
		auto fiber = ScheduleOut()
		o.(*f)(&o, args, [fiber](const boost::system::error_code& ec, std::size_t bytes_transferred) {
			ScheduleOutFinal(
				ScheduleIn(fiber, std::move(body), bytes_transferred);
			)
		});
	}
}
