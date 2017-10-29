#pragma once

#include <boost/asio.hpp>
#include "../omniengine/Instance.h"
#include "../omniengine/AsyncCall.h"

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace Omni {
	class EntityTcpListener;
	class InstanceTcpListener : public Instance {
	public:
		InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpListener();

		virtual void start(boost::asio::io_service& io, Completion<> complete);
		virtual void stop(boost::asio::io_service& io, Completion<> complete);
	private:
		std::shared_ptr<EntityTcpListener> entity;
		boost::asio::ip::tcp::socket sock;
	};
}
