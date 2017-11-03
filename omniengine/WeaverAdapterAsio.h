#pragma once

#include <tuple>
#include <functional>
#include <boost/core/noncopyable.hpp>
#include <boost/mpl/identity.hpp>

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

			template<typename ... Result> using ProvidedHandler = typename std::function<Fiber(Result ...)>;
			template<typename ... Result> using AsioHandler = typename std::function<void(const boost::system::error_code& ec, Result ...)>;
			template<typename ... Result> using Transformer = std::function<AsioHandler<Result ...>(ProvidedHandler<Result ...>&&)>;

			template<typename ... Result>
			Fiber yield(typename boost::mpl::identity<std::function<void(Transformer<Result ...>&&)>>::type && body) {
				return ::Omni::Fiber::yield([&](::Omni::Fiber::Restart&& restart) -> void {
					body([&](ProvidedHandler<Result ...>&& wrapped) -> AsioHandler<Result ...> {
						return [restart = std::move(restart), wrapped = std::move(wrapped)](const boost::system::error_code& ec, Result && ... result) {
							return restart([&]() -> Fiber {
								if (ec) throw ExceptionUnhandledError(ec);
								return wrapped(std::forward<Result>(result)...);
							});
						};
					});
				});
			};
		}
	}
}
