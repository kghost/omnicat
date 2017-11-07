#include "../common/header.h"

#include "InstanceTcpResolver.h"

#include "../omniengine/WeaverAdapterAsio.h"
#include "EntityTcpResolver.h"

namespace Omni {
	InstanceTcpResolver::InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io) : entity(entity), resolver(io) {}
	InstanceTcpResolver::~InstanceTcpResolver() {}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, boost::asio::ip::tcp::resolver::query &q, Completion<std::vector<InstanceResolver::EndpointT>&&> complete) {
		return Fiber::Asio::yield<boost::asio::ip::tcp::resolver::iterator>(
			[&](auto&& handler) {
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
		auto result = std::make_shared<std::vector<EndpointT>>();
		auto& familyies = entity->family.value();
		std::stack<Fiber::Fiber> children;
		std::for_each(familyies.begin(), familyies.end(), [&](auto family) {
			children.push(Fiber::fork([&](auto&& exit) {
				switch (family) {
					case decltype(entity)::element_type::Family::IPv4:
						{
							auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v4(), entity->host, entity->service, flags);
							return resolve(io, q, [result, exit = std::move(exit)](auto&& r) {
								result->insert(result->end(), std::make_move_iterator(r.begin()), std::make_move_iterator(r.end()));
								return exit();
							});
						}
					case decltype(entity)::element_type::Family::IPv6:
						{
							auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v6(), entity->host, entity->service, flags);
							return resolve(io, q, [exit = std::move(exit)](auto&& r) {
								return exit();
							});
						}
					default:
						OMNI_INTERNAL_ERROR;
				}
			}));
		});
		return Fiber::join(std::move(children), [result, complete = std::move(complete)] {
			return complete(std::move(*result));
		});
	}
}
