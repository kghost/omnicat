#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <boost/asio/ip/tcp.hpp>

#include "../omniengine/InstanceResolver.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class EntityTcpResolver;
	class InstanceTcpResolver : public InstanceResolver, public std::enable_shared_from_this<InstanceTcpResolver> {
	public:
		InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpResolver();

		using EndpointType = boost::asio::ip::basic_resolver_entry<boost::asio::ip::tcp>;
		using EndpointsType = std::vector<EndpointType>;

		virtual Fiber::Fiber resolve(boost::asio::io_service & io, bool passive, Completion<EndpointsT&&> complete);
	private:
		Fiber::Fiber resolve(boost::asio::io_service & io, boost::asio::ip::tcp::resolver::query &q, Completion<EndpointsT&&> complete);

		std::shared_ptr<EntityTcpResolver> entity;
		boost::asio::ip::tcp::resolver resolver;

		boost::log::sources::logger lg;
	};
}
