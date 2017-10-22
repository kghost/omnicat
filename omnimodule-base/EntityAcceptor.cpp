#include "EntityAcceptor.h"

#include <boost/asio.hpp>

namespace Omni {
	void EntityAcceptor::createInstance(boost::asio::io_service & io_service, std::function<void(std::shared_ptr<Instance>)> handler) {
		//boost::asio::ip::tcp::acceptor acceptor(io_service);
		//boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
		//acceptor.open(endpoint.protocol());
		//acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		//acceptor.bind(endpoint);
		//acceptor.listen();
	}
}
