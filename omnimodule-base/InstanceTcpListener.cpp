#include "../common/header.h"

#include "InstanceTcpListener.h"

#include "EntityTcpListener.h"
#include "../omniengine/Resolver.h"

namespace Omni {
	InstanceTcpListener::InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io)
		: entity(entity), sock(io) {}
	InstanceTcpListener::~InstanceTcpListener() {}

	void InstanceTcpListener::start(boost::asio::io_service & io, Completion<> complete) {
		complete.ok();
	}
	void InstanceTcpListener::stop(boost::asio::io_service & io, Completion<> complete) {
		complete.ok();
	}
}
