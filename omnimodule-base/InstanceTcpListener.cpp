#include "../common/header.h"

#include "InstanceTcpListener.h"

#include "EntityTcpListener.h"
#include "../omniengine/Resolver.h"
#include "../omniengine/InstanceResolver.h"

namespace Omni {
	InstanceTcpListener::InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io)
		: entity(entity), sock(io) {}
	InstanceTcpListener::~InstanceTcpListener() {}

	Fiber::Fiber InstanceTcpListener::start(boost::asio::io_service & io, Completion<> complete) {
		return entity->resolver->createInstance(io, [&io, complete = std::move(complete)](std::shared_ptr<InstanceResolver>&& r) {
			return r->resolve(io, true, [complete = std::move(complete)](auto addresses) {
				return complete();
			});
		});
	}
	Fiber::Fiber InstanceTcpListener::stop(boost::asio::io_service & io, Completion<> complete) { return complete(); }
}
