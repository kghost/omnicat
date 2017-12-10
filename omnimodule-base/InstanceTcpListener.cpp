#include "../common/header.h"

#include "InstanceTcpListener.h"

#include <type_traits>
#include <boost/asio/ip/tcp.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/trivial.hpp>

#include "EntityTcpListener.h"
#include "../omniengine/WeaverAdapterAsio.h"
#include "../omniengine/Resolver.h"
#include "InstanceTcpResolver.h"

namespace Omni {
	InstanceTcpListener::InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io) : entity(entity) {
		lg.add_attribute("Component", boost::log::attributes::constant<std::string>((boost::format("%1%(%2%)") % typeid(*this).name() % this).str()));
	}
	InstanceTcpListener::~InstanceTcpListener() {}

	class InstanceTcpListener::AcceptHandler : public std::enable_shared_from_this<AcceptHandler> {
	public:
		AcceptHandler(std::shared_ptr<InstanceTcpListener> instance, std::shared_ptr<InstanceTcpListener::Acceptor> acceptor, Fiber::ContinuationRef&& exit)
			: instance(instance), acceptor(acceptor), exit(std::move(exit)) {}

		Fiber::Fiber operator()(boost::asio::io_service & io) {
			auto o = acceptor->acceptor;
			if (!o->is_open()) return exit();
			return Fiber::Asio::yield<>([&](auto&& handler) {
				auto peer = std::make_shared<boost::asio::ip::tcp::socket>(io);
				auto peer_endpoint = std::make_shared<std::remove_reference<decltype(o)>::type::element_type::endpoint_type>();
				o->async_accept(*peer, *peer_endpoint, handler([&io, me = shared_from_this(), peer, peer_endpoint] {
					BOOST_LOG_SEV(me->instance->lg, boost::log::trivial::severity_level::debug) << "Accepted[" << me->acceptor.get() << "]: " << *peer_endpoint;
					// TOTO: handle peer
					return (*me)(io);
				}));
			});
		}

		std::shared_ptr<InstanceTcpListener> instance;
		std::shared_ptr<Acceptor> acceptor;
		Fiber::Continuation exit;
	};

	Fiber::Fiber InstanceTcpListener::start(boost::asio::io_service & io, Completion<> complete) {
		return entity->resolver->createInstance(io, [me = shared_from_this(), &io, complete = std::move(complete)](std::shared_ptr<InstanceResolver>&& r) {
			return r->resolve(io, true, [me, &io, complete = std::move(complete)](auto addresses) {
				auto as = boost::any_cast<InstanceTcpResolver::EndpointsType>(addresses);

				boost::log::record rec = me->lg.open_record(boost::log::keywords::severity = boost::log::trivial::severity_level::info);
				boost::log::record_ostream strm(rec);
				if (rec) {
					strm << " Listening on:";
				}

				for (auto& i : as) {
					auto o = std::make_shared<boost::asio::ip::tcp::acceptor>(io, i);
					auto acceptor = std::make_shared<Acceptor>(o);
					if (rec) strm << ' ' << i << "[" << acceptor.get() << "]";
					acceptor->fiber = Fiber::fork(boost::str(boost::format("listener %p") % acceptor.get()), [&](auto&& exit) {
						return Fiber::handle([&](auto&& exit2) {
							auto handler = std::make_shared<AcceptHandler>(me, acceptor, std::move(exit2));
							return (*handler)(io);
						}, [](std::exception_ptr && eptr, Fiber::ContinuationRef continuation) {
							try {
								std::rethrow_exception(eptr);
							} catch (const Fiber::Asio::ExceptionUnhandledError & e) {
								if (e.ec == boost::system::errc::operation_canceled) return continuation();
								throw;
							}
						}, std::move(exit));
					});
					me->acceptors.push_back(*acceptor);
				}

				if (rec) {
					strm.flush();
					me->lg.push_record(boost::move(rec));
				}

				return complete();
			});
		});
	}

	Fiber::Fiber InstanceTcpListener::stop(boost::asio::io_service & io, Completion<> complete) {
		for (auto & i : acceptors) i.acceptor->close();

		auto fs = std::make_shared<std::list<Fiber::Fiber>>();
		for (auto & i : acceptors) fs->push_back(i.fiber);
		return Fiber::join(fs->begin(), fs->end(), [fs, complete = std::move(complete)]{
			return complete();
		});
	}
}
