#pragma once

#include <tuple>
#include <boost/asio.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Resolver.h"
#include "OptionsResolver.h"

namespace Omni {
	class Registry;
	class EntityTcpResolver : public Resolver {
	public:
		EntityTcpResolver(std::shared_ptr<Registry> registry) : registry(registry), options(*this) {}
		virtual bool isPassive() { return false; };
		virtual void prepare() {}

		virtual void createInstance(std::function<int()> callback);
		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback);

	public:
		// needed by parser
		std::tuple<
			OptionsResolver<EntityTcpResolver>::Impl
		> options;

		Parser::Type groupOptionType(const std::string & key);
		void setRawOption(const std::string & value);
		void setOption(const std::string & key);
		void setOption(const std::string & key, const std::string & value);

		//#begin-region static configure fields before start
		std::vector<int> domains = {};
		std::string address = "localhost";
		std::string service = "omnicat";
		unsigned short port = -1;
		//#end-region

		std::shared_ptr<Registry> getRegistry() { return registry; }
	private:
		std::shared_ptr<Registry> registry;

		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
