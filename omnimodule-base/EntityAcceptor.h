#pragma once

#include <list>

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
		virtual bool isPassive() { return true; };

		virtual void createInstance(boost::asio::io_service & io_service, std::function<void(std::shared_ptr<Instance>)> handler);
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
