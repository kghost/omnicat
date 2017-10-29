#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <boost/asio/ip/tcp.hpp>

#include "../omniengine/InstanceResolver.h"
#include "../omniengine/AsyncCall.h"

namespace Omni {
	class EntityTcpResolver;
	class InstanceTcpResolver : public InstanceResolver, public std::enable_shared_from_this<InstanceTcpResolver> {
	public:
		InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpResolver();

		virtual void resolve(boost::asio::io_service & io, Completion<std::vector<InstanceResolver::EndpointT>&&> complete);
	private:
		std::shared_ptr<EntityTcpResolver> entity;
		boost::asio::ip::tcp::resolver resolver;
	};
}
