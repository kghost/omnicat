#pragma once

#include <tuple>
#include <set>
#include <optional>
#include <boost/asio/io_service.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Resolver.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class Registry;
	class InstanceResolver;
	class InstanceTcpResolver;
	class EntityTcpResolver : public Resolver, public std::enable_shared_from_this<EntityTcpResolver> {
	public:
		EntityTcpResolver(std::shared_ptr<Registry> registry) : Resolver(registry) {}
		virtual void prepare() {
			if (!family) family = {Family::IPv4, Family::IPv6};
			if (!hasHost) hasHost = false;
			if (!hasService) hasService = false;
		}

		typedef InstanceTcpResolver InstanceType;

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<InstanceResolver>> complete);
	public:
		// needed by parser
		Parser::Type groupOptionType(const std::string & key);
		void setRawOption(const std::string & value);
		void setOption(const std::string & key);
		void setOption(const std::string & key, const std::string & value);

		//#begin-region static configure fields before start
		enum class Family { IPv4, IPv6 };
		std::optional<std::set<Family>> family = {};
		std::optional<bool> hasHost;
		std::optional<bool> hasService;
		std::string host = "localhost";
		std::string service = "omnicat";
		//#end-region
	private:
		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
