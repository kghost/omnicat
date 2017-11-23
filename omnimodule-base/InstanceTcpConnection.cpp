#include "../common/header.h"

#include "InstanceTcpConnection.h"

#include <type_traits>
#include <boost/asio/ip/tcp.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/trivial.hpp>

#include "EntityTcpConnection.h"
#include "../omniengine/WeaverAdapterAsio.h"
#include "../omniengine/Resolver.h"
#include "InstanceTcpResolver.h"

namespace Omni {
	InstanceTcpConnection::InstanceTcpConnection(std::shared_ptr<EntityTcpConnection> entity, boost::asio::io_service & io) : entity(entity) {
		lg.add_attribute("Component", boost::log::attributes::constant<std::string>((boost::format("%1%(%2%)") % typeid(*this).name() % this).str()));
	}
	InstanceTcpConnection::~InstanceTcpConnection() {}

	Fiber::Fiber InstanceTcpConnection::start(boost::asio::io_service & io, Completion<> complete) {
		return complete();
	}

	Fiber::Fiber InstanceTcpConnection::stop(boost::asio::io_service & io, Completion<> complete) {
		return complete();
	}
}
