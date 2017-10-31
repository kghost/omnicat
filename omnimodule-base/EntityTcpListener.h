#pragma once

#include <tuple>
#include <boost/asio.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Weave.h"
#include "OptionsTcpListener.h"
#include "OptionsListener.h"

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace Omni {
	class Registry;
	class Resolver;
	class EntityTcpListener : public Entity, public std::enable_shared_from_this<EntityTcpListener>{
	public:
		EntityTcpListener(std::shared_ptr<Registry> registry) : registry(registry), options{*this, *this} {}
		virtual void prepare();

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete);
	public:
		std::shared_ptr<Registry> getRegistry() { return registry; }

		// needed by parser
		Parser::Type groupOptionType(const std::string & key);
		void setRawOption(const std::string & value);
		void setOption(const std::string & key);
		void setOption(const std::string & key, const std::string & value);

		//#begin-region static configure fields before start
		std::tuple<
			OptionsTcpListener<EntityTcpListener, boost::asio::ip::tcp::socket::native_handle_type>::Impl,
			OptionsListener<EntityTcpListener>::Impl
		> options;
		

		std::shared_ptr<Resolver> resolver;
		std::shared_ptr<Entity> pipeline;
		unsigned int limit = 1;
		//#end-region
	private:
		std::shared_ptr<Registry> registry;

		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
