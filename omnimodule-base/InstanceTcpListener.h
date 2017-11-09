#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

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

		boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	private:
		std::shared_ptr<EntityTcpListener> entity;

		std::list<std::tuple<
			boost::asio::ip::tcp::endpoint,
			std::shared_ptr<boost::asio::ip::tcp::acceptor>,
			Fiber::Fiber
		>> acceptors;
	};
}
