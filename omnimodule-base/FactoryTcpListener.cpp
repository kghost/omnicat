#include "../common/header.h"

#include "FactoryTcpListener.h"

#include "memory"

#include "../omniengine/ParserHelper.h"
#include "EntityTcpListener.h"

namespace Omni {
	std::shared_ptr<Parser::Object> FactoryTcpListener::createObject() {
		return std::make_shared<Parser::EntityWrapper<
			EntityTcpListener,
			Parser::HelperObject<EntityTcpListener, Parser::HelperGroup<EntityTcpListener, true, true>>
		>>(getRegistry());
	}

	std::shared_ptr<Entity> FactoryTcpListener::createEntity() {
		return std::make_shared<EntityTcpListener>(getRegistry());
	}
}
