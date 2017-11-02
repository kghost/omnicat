#pragma once

#include <tuple>
#include <set>
#include <boost/asio/io_service.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Resolver.h"
#include "../omniengine/Weave.h"
#include "OptionsResolver.h"

namespace Omni {
	class Registry;
	class InstanceResolver;
	class EntityTcpResolver : public Resolver, public std::enable_shared_from_this<EntityTcpResolver> {
	public:
		EntityTcpResolver(std::shared_ptr<Registry> registry) : registry(registry), options(*this) {}
		virtual void prepare() {
			if (!family) family = {Family::IPv4, Family::IPv6};
			if (!hasHost) hasHost = false;
			if (!hasService) hasService = false;
		}

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<InstanceResolver>> complete);
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
		enum class Family { IPv4, IPv6 };
		std::optional<std::set<Family>> family = {};
		std::optional<bool> hasHost;
		std::optional<bool> hasService;
		std::string host = "localhost";
		std::string service = "omnicat";
		//#end-region

		std::shared_ptr<Registry> getRegistry() { return registry; }
	private:
		std::shared_ptr<Registry> registry;

		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
