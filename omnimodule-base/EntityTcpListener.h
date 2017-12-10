#pragma once

#include <tuple>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class Resolver;
	class EntityPipeline;
	class EntityTcpConnection;
	class InstanceTcpListener;
	class EntityTcpListener : public Entity, public std::enable_shared_from_this<EntityTcpListener>{
	public:
		EntityTcpListener(std::shared_ptr<Registry> registry) : Entity(registry) {}
		virtual void prepare();

		typedef InstanceTcpListener InstanceType;

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete);
	public:
		// needed by parser
		Parser::Type groupOptionType(const std::string & key);
		void setRawOption(const std::string & value);
		void setOption(const std::string & key);
		void setOption(const std::string & key, const std::string & value);

		//{ static configure fields before start
		std::shared_ptr<Resolver> resolver;
		std::shared_ptr<EntityPipeline> pipeline;
		std::shared_ptr<EntityTcpConnection> connection;
		unsigned int limit = 1;
		//}
	private:
		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
