#pragma once

#include <tuple>
#include <boost/asio.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "OptionsTcpListener.h"
#include "OptionsListener.h"

namespace Omni {
	class Registry;
	class Resolver;
	class EntityTcpListener : public Entity {
	public:
		EntityTcpListener(std::shared_ptr<Registry> registry) : registry(registry), options{*this, *this} {}
		virtual bool isPassive() { return false; };
		virtual void prepare() {}

		virtual void createInstance(std::function<int()> callback);
		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback);

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
