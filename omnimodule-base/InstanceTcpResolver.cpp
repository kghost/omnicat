#include "../common/header.h"

#include "InstanceTcpResolver.h"

namespace Omni {
	InstanceTcpResolver::InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io) : entity(entity), resolver(io) {}
	InstanceTcpResolver::~InstanceTcpResolver() {}

	void InstanceTcpResolver::resolve(boost::asio::io_service & io, Completion<std::vector<EndpointT>&&> complete) {
		//auto handler = [me = shared_from_this(), &io](const ErrorCode::error_code& ec, boost::asio::ip::tcp::resolver::iterator iterator) {
		//	if (!ec) {
		//		for (decltype(iterator) iend; iterator != iend; ++iterator) {
		//			auto endpoint = iterator->endpoint();
		//			if (endpoint.address() == boost::asio::ip::address_v4::any() || endpoint.address() == boost::asio::ip::address_v6::any()) {
		//				for (auto &a : addresses) {
		//					if (a.second.protocol() == endpoint.protocol()) {
		//						decltype(a.second) e(a.second);
		//						e.port(endpoint.port());
		//						BOOST_LOG_TRIVIAL(info) << "Listening on interface(" << a.first << "): " << e;
		//						std::make_shared<listener>(io_service)->start(e);
		//					}
		//				}
		//			} else {
		//				BOOST_LOG_TRIVIAL(info) << "Listining: " << endpoint << " from " << end;
		//				std::make_shared<listener>(io_service)->start(endpoint);
		//			}
		//		}
		//	} else {
		//		BOOST_LOG_TRIVIAL(error) << "Error resolve endpoint(" << end << "): " << ec.message();
		//	}
		//};

		//auto s = end.rfind(':');
		//if (s == std::string::npos) {
		//	resolver->async_resolve({end}, handler);
		//} else {
		//	resolver->async_resolve({end.substr(0, s), end.substr(s + 1)}, handler);
		//}
	}
}
