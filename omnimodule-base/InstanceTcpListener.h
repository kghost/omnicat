#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/sources/logger.hpp>

#include "../omniengine/Instance.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class EntityTcpListener;
	class InstanceTcpListener : public std::enable_shared_from_this<InstanceTcpListener>, public Instance {
	public:
		InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpListener();

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);
	private:
		std::shared_ptr<EntityTcpListener> entity;
		boost::asio::ip::tcp::socket sock;

		boost::log::sources::logger lg;
	};
}
