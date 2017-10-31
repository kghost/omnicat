#pragma once

#include <boost/asio.hpp>
#include "../omniengine/Instance.h"
#include "../omniengine/Weave.h"

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

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);
	private:
		std::shared_ptr<EntityTcpListener> entity;
		boost::asio::ip::tcp::socket sock;
	};
}
