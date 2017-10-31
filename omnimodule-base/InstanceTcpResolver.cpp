#include "../common/header.h"

#include "InstanceTcpResolver.h"

#include "../omniengine/WeaverAdapterAsio.h"
#include "EntityTcpResolver.h"

namespace Omni {
	InstanceTcpResolver::InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io) : entity(entity), resolver(io) {}
	InstanceTcpResolver::~InstanceTcpResolver() {}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, bool passive, Completion<std::vector<EndpointT>> complete) {
		auto o = std::make_shared<boost::asio::ip::tcp::resolver>(io);
		auto flags = boost::asio::ip::resolver_query_base::flags();
		if (passive) flags |= boost::asio::ip::resolver_query_base::flags::passive;
		if (entity->hasHost.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_host;
		if (entity->hasService.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_service;
		auto family = entity->family.value();
		if (family.find(decltype(entity)::element_type::Family::IPv4) != family.end()) {
			auto q = decltype(o)::element_type::query(boost::asio::ip::tcp::v4(), entity->host, entity->service, flags);
			return Fiber::Asio::yield<boost::asio::ip::tcp::resolver::iterator>(
				[&](auto handler) {
					o->async_resolve(q, handler([complete = std::move(complete)](boost::asio::ip::tcp::resolver::iterator iterator) {
						return complete(std::vector<EndpointT>());
					}));
				}
			);
		} else {
			return complete(std::vector<EndpointT>());
		}
	}
}
