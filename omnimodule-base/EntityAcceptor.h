#pragma once

#include <list>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Resolver.h"

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

		virtual void createInstance(boost::asio::io_service & io_service, std::function<void(std::shared_ptr<Instance>)> handler);

		Parser::Type groupOptionType(const std::string & key) {
			return Parser::Type::STRING;
		}

		bool setRawOption(const std::string & value) {
			return true;
		}
		bool setOption(const std::string & key) {
			return true;
		}
		bool setOption(const std::string & key, const std::string & value) {
			return true;
		}
	private:
		//#region static configure fields before start

		std::shared_ptr<Resolver> resolver;

		// Entity to pair with accepted connection
		// Acceptor(SocketRecvSend - <inner>) 
		std::shared_ptr<Entity> inner;

		//#endregion

		//#region runtime states
		std::list<std::shared_ptr<Instance>> instances;
		//#endregion
	};
}
