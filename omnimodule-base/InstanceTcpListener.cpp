#include "../common/header.h"

#include "InstanceTcpListener.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/trivial.hpp>

#include "EntityTcpListener.h"
#include "../omniengine/WeaverAdapterAsio.h"
#include "../omniengine/Resolver.h"
#include "InstanceTcpResolver.h"

namespace Omni {
	InstanceTcpListener::InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io)
		: entity(entity) {
		lg.add_attribute("Component", boost::log::attributes::constant<std::string>((boost::format("%1%(%2%)") % typeid(*this).name() % this).str()));
	}
	InstanceTcpListener::~InstanceTcpListener() {}

	class AcceptHandler : public std::enable_shared_from_this<AcceptHandler> {
	public:
		AcceptHandler(std::shared_ptr<InstanceTcpListener> instance,
			std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, Fiber::Continuation&& exit)
			: instance(instance), acceptor(acceptor), exit(std::move(exit)) {}

		Fiber::Fiber operator()(boost::asio::io_service & io) {
		if (!acceptor->is_open()) return exit();
			return Fiber::Asio::yield<>([&](auto&& handler) {
				auto peer = std::make_shared<boost::asio::ip::tcp::socket>(io);
				auto peer_endpoint = std::make_shared<decltype(acceptor)::element_type::endpoint_type>();
				acceptor->async_accept(*peer, *peer_endpoint, handler([&io, me = shared_from_this(), peer, peer_endpoint]{
					boost::log::record rec = me->instance->lg.open_record(boost::log::keywords::severity = boost::log::trivial::severity_level::debug);
					if (rec) {
						boost::log::record_ostream strm(rec);
						strm << " Accepted[" << me->acceptor.get() << "]: " << peer_endpoint;
						strm.flush();
						me->instance->lg.push_record(boost::move(rec));
					}

					return (*me)(io);
				}));
			});
		}

		std::shared_ptr<InstanceTcpListener> instance;
		std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
		Fiber::Continuation exit;
	};

	Fiber::Fiber InstanceTcpListener::start(boost::asio::io_service & io, Completion<> complete) {
		return entity->resolver->createInstance(io, [me = shared_from_this(), &io, complete = std::move(complete)](std::shared_ptr<InstanceResolver>&& r) {
			return r->resolve(io, true, [me, &io, complete = std::move(complete)](auto addresses) {
				auto as = boost::any_cast<InstanceTcpResolver::EndpointsType>(addresses);

				for (auto& i : as) {
					auto acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(io, i.endpoint());
					me->acceptors.push_back({
						i.endpoint(), acceptor, Fiber::fork([&](auto&& exit) {
							auto handler = std::make_shared<AcceptHandler>(me->shared_from_this(), acceptor, std::move(exit));
							return (*handler)(io);
						})
					});
				}

				boost::log::record rec = me->lg.open_record(boost::log::keywords::severity = boost::log::trivial::severity_level::info);
				if (rec) {
					boost::log::record_ostream strm(rec);
					strm << " Listening on:";
					for (auto i : me->acceptors) strm << ' ' << std::get<0>(i) << "[" << std::get<1>(i).get() << "]";
					strm.flush();
					me->lg.push_record(boost::move(rec));
				}

				return complete();
			});
		});
	}
	Fiber::Fiber InstanceTcpListener::stop(boost::asio::io_service & io, Completion<> complete) {
		return complete();
	}
}
