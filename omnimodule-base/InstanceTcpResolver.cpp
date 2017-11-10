#include "../common/header.h"

#include "InstanceTcpResolver.h"

#include <list>
#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/trivial.hpp>

#include "../omniengine/WeaverAdapterAsio.h"
#include "EntityTcpResolver.h"

namespace Omni {
	InstanceTcpResolver::InstanceTcpResolver(std::shared_ptr<EntityTcpResolver> entity, boost::asio::io_service & io) : entity(entity), resolver(io) {
		lg.add_attribute("Component", boost::log::attributes::constant<std::string>((boost::format("%1%(%2%)") % typeid(*this).name() % this).str()));
	}
	InstanceTcpResolver::~InstanceTcpResolver() {}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, boost::asio::ip::tcp::resolver::query &q, Completion<EndpointsT&&> complete) {
		auto u = std::rand();
		BOOST_LOG_SEV(lg, boost::log::trivial::severity_level::debug) << " Resolving[" << u << "]: family(" << q.hints().ai_family << "),type(" << q.hints().ai_socktype
			<< "),protocol(" << q.hints().ai_protocol << "),flags(" << q.hints().ai_flags << ") "
			<< q.host_name() << ":" << q.service_name();

		return Fiber::Asio::yield<boost::asio::ip::tcp::resolver::iterator>([&](auto&& handler) {
			auto o = std::make_shared<boost::asio::ip::tcp::resolver>(io);
			o->async_resolve(q, handler([me = shared_from_this(), complete = std::move(complete), u, o](boost::asio::ip::tcp::resolver::iterator iterator) {
				typename InstanceTcpResolver::EndpointsType v;
				auto end = decltype(iterator)();
				std::for_each(iterator, end, [&](auto& i) { v.insert(i); });

				boost::log::record rec = me->lg.open_record(boost::log::keywords::severity = boost::log::trivial::severity_level::debug);
				if (rec) {
					boost::log::record_ostream strm(rec);
					strm << " Resolved[" << u << "]:";
					for (auto i : v) strm << " " << i;
					strm.flush();
					me->lg.push_record(boost::move(rec));
				}

				return complete(v);
			}));
		});
	}

	Fiber::Fiber InstanceTcpResolver::resolve(boost::asio::io_service & io, bool passive, Completion<EndpointsT&&> complete) {
		auto flags = boost::asio::ip::resolver_query_base::flags();
		if (passive) flags |= boost::asio::ip::resolver_query_base::flags::passive;
		if (entity->hasHost.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_host;
		if (entity->hasService.value()) flags |= boost::asio::ip::resolver_query_base::flags::numeric_service;
		auto result = std::make_shared<EndpointsType>();
		auto& familyies = entity->family.value();
		auto children = std::make_shared<std::list<Fiber::Fiber>>();
		for (auto& family : familyies) {
			children->push_back(Fiber::fork(boost::str(boost::format("resolver %p") % this), [&](auto&& exit) {
				switch (family) {
					case decltype(entity)::element_type::Family::IPv4:
					{
						auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v4(), entity->host, entity->service, flags);
						return resolve(io, q, [result, exit = std::move(exit)](auto&& r) {
							for (auto& i : boost::any_cast<EndpointsType>(r)) result->insert(i);
							return exit();
						});
					}
					case decltype(entity)::element_type::Family::IPv6:
					{
						auto q = boost::asio::ip::tcp::resolver::query(boost::asio::ip::tcp::v6(), entity->host, entity->service, flags);
						return resolve(io, q, [result, exit = std::move(exit)](auto&& r) {
							for (auto& i : boost::any_cast<EndpointsType>(r)) result->insert(i);
							return exit();
						});
					}
					default:
						OMNI_INTERNAL_ERROR;
				}
			}));
		};
		return Fiber::join(children->begin(), children->end(), [children, result, complete = std::move(complete)]{
			return complete(std::move(*result));
		});
	}
}
