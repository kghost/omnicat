#pragma once

#include <list>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "../omniengine/Instance.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class EntityTcpConnection;
	class InstanceTcpConnection : public std::enable_shared_from_this<InstanceTcpConnection>, public Instance {
	public:
		InstanceTcpConnection(std::shared_ptr<EntityTcpConnection> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpConnection();

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);

		boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	private:
		std::shared_ptr<EntityTcpConnection> entity;
	};
}
