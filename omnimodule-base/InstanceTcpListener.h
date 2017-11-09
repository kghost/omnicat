#pragma once

#include <list>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "../omniengine/Instance.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class EntityTcpListener;
	class InstanceTcpListener : public std::enable_shared_from_this<InstanceTcpListener>, public Instance {
	public:
		InstanceTcpListener(std::shared_ptr<EntityTcpListener> entity, boost::asio::io_service & io);
		virtual ~InstanceTcpListener();

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);

		boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
	private:
		std::shared_ptr<EntityTcpListener> entity;

		class AcceptHandler;
		class Acceptor : private boost::noncopyable {
			public:
				Acceptor(std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor) : acceptor(acceptor) {}

				boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> hook_;

				std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
				Fiber::Fiber fiber;
		};

		boost::intrusive::list<Acceptor, boost::intrusive::constant_time_size<false>, boost::intrusive::member_hook<Acceptor, boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>, &Acceptor::hook_>> acceptors;
	};
}
