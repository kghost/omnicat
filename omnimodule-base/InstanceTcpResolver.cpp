#include "../common/header.h"

#include "InstanceTcpResolver.h"

#include "../omniengine/WeaverAdapterAsio.h"
#include "EntityTcpResolver.h"

namespace Omni {
	InstanceTcpResolver::InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io) : entity(entity), resolver(io) {}
	InstanceTcpResolver::~InstanceTcpResolver() {}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, boost::asio::ip::tcp::resolver::query &q, Completion<std::vector<InstanceResolver::EndpointT>&&> complete) {
		return Fiber::Asio::yield<boost::asio::ip::tcp::resolver::iterator>(
			[&](auto&& handler) -> void {
				auto o = std::make_shared<boost::asio::ip::tcp::resolver>(io);
				o->async_resolve(q, handler([complete = std::move(complete), o](boost::asio::ip::tcp::resolver::iterator iterator) {
					return complete(std::vector<EndpointT>());
				}));
			}
		);
	}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, bool passive, Completion<std::vector<EndpointT>> complete) {
		auto flags = boost::asio::ip::resolver_query_base::flags();
		if (passive) flags |= boost::asio::ip::resolver_query_base::flags::passive;
		if (entity->hasHost.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_host;
		if (entity->hasService.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_service;
		auto result = std::unique_ptr<std::vector<EndpointT>>();
		auto& familyies = entity->family.value();
		std::vector<Fiber::Fiber> children;
		std::transform(familyies.begin(), familyies.end(), std::back_inserter(children),
			[&](auto family) {
				return Fiber::fork([&](auto&& exit) {
					switch (family) {
						case decltype(entity)::element_type::Family::IPv4:
							{
								auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v4(), entity->host, entity->service, flags);
								return resolve(io, q, [exit = std::move(exit)](auto&& result) { return exit(); });
							}
						case decltype(entity)::element_type::Family::IPv6:
							{
								auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v6(), entity->host, entity->service, flags);
								return resolve(io, q, [exit = std::move(exit)](auto&& result) { return exit(); });
							}
						default:
							OMNI_INTERNAL_ERROR;
					}
				});
			});
		return complete(std::move(*result.release()));
	}
}
