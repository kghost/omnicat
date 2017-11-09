#pragma once

#include <memory>
#include <set>
#include <functional>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "../omniengine/InstanceResolver.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class EntityTcpResolver;
	class InstanceTcpResolver : public InstanceResolver, public std::enable_shared_from_this<InstanceTcpResolver> {
	public:
		InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpResolver();

		using EndpointType = boost::asio::ip::tcp::endpoint;
		using EndpointsType = std::set<EndpointType>;

		virtual Fiber::Fiber resolve(boost::asio::io_service & io, bool passive, Completion<EndpointsT&&> complete);
	private:
		Fiber::Fiber resolve(boost::asio::io_service & io, boost::asio::ip::tcp::resolver::query &q, Completion<EndpointsT&&> complete);

		std::shared_ptr<EntityTcpResolver> entity;
		boost::asio::ip::tcp::resolver resolver;

		boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	};
}
