#include "../common/header.h"

#include "InstanceTcpListener.h"

#include <boost/format.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/trivial.hpp>

#include "EntityTcpListener.h"
#include "../omniengine/Resolver.h"
#include "InstanceTcpResolver.h"

namespace Omni {
	InstanceTcpListener::InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io)
		: entity(entity), sock(io) {
			lg.add_attribute("Component", boost::log::attributes::constant<std::string>((boost::format("%1%(%2%)") % typeid(*this).name() % this).str()));
		}
	InstanceTcpListener::~InstanceTcpListener() {}

	Fiber::Fiber InstanceTcpListener::start(boost::asio::io_service & io, Completion<> complete) {
		return entity->resolver->createInstance(io, [me = shared_from_this(), &io, complete = std::move(complete)](std::shared_ptr<InstanceResolver>&& r) {
			return r->resolve(io, true, [me = me->shared_from_this(), complete = std::move(complete)](auto addresses) {
				auto as = boost::any_cast<InstanceTcpResolver::EndpointsType>(addresses);

				boost::log::record rec = me->lg.open_record(boost::log::keywords::severity = boost::log::trivial::severity_level::info);
				if (rec) {
					boost::log::record_ostream strm(rec);
					strm << " Listening on:";
					for (auto i : as) strm << ' ' << i.endpoint();
					me->lg.push_record(boost::move(rec));
				}

				return complete();
			});
		});
	}
	Fiber::Fiber InstanceTcpListener::stop(boost::asio::io_service & io, Completion<> complete) { return complete(); }
}
