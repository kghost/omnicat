#pragma once

#include <functional>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "Weave.h"
#include "shared.h"

namespace Omni {
	class SHARED Instance : private boost::noncopyable {
	public:
		virtual ~Instance() {}
		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete) = 0;
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete) = 0;
	};
}
