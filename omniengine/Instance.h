#pragma once

#include <functional>
#include <boost/core/noncopyable.hpp>

#include "AsyncCall.h"
#include "shared.h"

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace Omni {
	class SHARED Instance : private boost::noncopyable {
	public:
		virtual ~Instance() {}
		virtual void start(boost::asio::io_service& io, Completion<> complete) = 0;
		virtual void stop(boost::asio::io_service& io, Completion<> complete) = 0;
	};
}
