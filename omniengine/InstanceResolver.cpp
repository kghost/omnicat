#include "../common/header.h"

#include "InstanceResolver.h"

namespace Omni {
	InstanceResolver::InstanceResolver() {}
	InstanceResolver::~InstanceResolver() {}
	Fiber::Fiber InstanceResolver::start(boost::asio::io_service & io, Completion<> complete) { return complete(); }
	Fiber::Fiber InstanceResolver::stop(boost::asio::io_service & io, Completion<> complete) { return complete(); }
}
