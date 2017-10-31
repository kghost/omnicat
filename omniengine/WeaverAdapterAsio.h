#pragma once

#include <boost/core/noncopyable.hpp>

#include "Weave.h"
#include "Exception.h"
#include "ErrorCode.h"

#include "shared.h"

namespace Omni {
	namespace Fiber {
		namespace Asio {
			class SHARED ExceptionUnhandledError : public Exception {
			public:
				ExceptionUnhandledError(ErrorCode::error_code ec);
				virtual ~ExceptionUnhandledError();

				virtual const char* what() const noexcept;
			private:
#pragma warning(push)
#pragma warning(disable: 4251)
				ErrorCode::error_code ec;
				std::string w;
#pragma warning(pop)
			};

			template<typename ... Result>
			Fiber yield(std::function<void(std::function<std::function<void(const boost::system::error_code& ec, Result ...)>(std::function<Fiber(Result ...)>)>&&)>&& body) {
				auto fiber = ::Omni::Fiber::ScheduleOut();
				body([&](std::function<Fiber(Result ...)>&& wrapped) -> std::function<void(const boost::system::error_code& ec, Result ...)> {
					return [fiber, wrapped = std::move(wrapped)](const boost::system::error_code& ec, Result && ... result) -> void {
						fiber->restart([&]{
							if (ec) throw ExceptionUnhandledError(ec);
							return wrapped(std::forward<Result>(result)...);
						});
					};
				});
				return fiber;
			};
		}
	}
}
