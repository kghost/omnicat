#pragma once

#include <utility>
#include <functional>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "ErrorCode.h"
#include "Exception.h"

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

	template<typename ... ResultT>
	class Completion {
	public:
		template<typename SuccessT, typename FailT>
		Completion(SuccessT && successF, FailT && failF)
			: success(std::forward<SuccessT>(successF)), fail(std::forward<FailT>(failF)) {}

		void ok(ResultT && ... result) const {
			in([this, result = std::make_tuple(std::forward<ResultT>(result)...)]{
				std::apply(success, result);
			});
		}

		void in(std::function<void()>&& fun) const {
			std::exception_ptr eptr;
			try { return fun(); } catch (const Exception &) {
				eptr = std::current_exception();
			}
			fail(eptr);
		}
#define V(ec) v(ec, "(@" __FILE__ ":"  BOOST_PP_STRINGIZE(__LINE__) ")")
		void v(ErrorCode::error_code ec, std::string location) const {
			if (ec) throw ExceptionUnhandledError(ec, std::move(location));
		}

		std::function<void(ResultT ...)> success;
		std::function<void(std::exception_ptr)> fail;
	};
}
