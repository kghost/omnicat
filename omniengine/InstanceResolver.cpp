#include "../common/header.h"

#include "InstanceResolver.h"

namespace Omni {
	InstanceResolver::InstanceResolver() {}
	InstanceResolver::~InstanceResolver() {}
	void InstanceResolver::start(boost::asio::io_service & io, Completion<> complete) { complete.ok(); }
	void InstanceResolver::stop(boost::asio::io_service & io, Completion<> complete) { complete.ok(); }
}
