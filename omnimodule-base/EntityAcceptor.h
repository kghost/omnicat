#pragma once

#include <boost/fusion/include/list.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Resolver.h"
#include "OptionsFileDescriptor.h"
#include "OptionsSocketListener.h"

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace Omni {
	class EntityAcceptor : public Entity {
	public:
		virtual bool isPassive() { return false; };
		virtual void prepare() {}

		virtual void createInstance(std::function<int()> callback);
		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback);

		Parser::Type groupOptionType(const std::string & key);
		bool setRawOption(const std::string & value);
		bool setOption(const std::string & key);
		bool setOption(const std::string & key, const std::string & value);
	private:
		//#begin-region static configure fields before start

		boost::fusion::list<
			OptionsSocketListener,
			OptionsFileDescriptor
		> options;

		std::shared_ptr<Resolver> resolver;

		// Entity to pair with accepted connection
		// Acceptor(SocketRecvSend - <inner>) 
		std::shared_ptr<Entity> inner;

		//#end-region

		//#begin-region runtime states
		//std::list<std::shared_ptr<Instance>> instances;
		//#end-region
	};
}
