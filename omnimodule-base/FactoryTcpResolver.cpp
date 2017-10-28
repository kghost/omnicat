#include "../common/header.h"

#include "FactoryTcpResolver.h"

#include "memory"

#include "../omniengine/ParserHelper.h"
#include "EntityTcpResolver.h"

namespace Omni {
	std::shared_ptr<Parser::Object> FactoryTcpResolver::createObject() {
		return std::make_shared<Parser::EntityWrapper<
			EntityTcpResolver,
			Parser::HelperObject<EntityTcpResolver, Parser::HelperGroup<EntityTcpResolver, true, true>>
		>>(getRegistry());
	}

	std::shared_ptr<Entity> FactoryTcpResolver::createEntity() {
		return std::make_shared<EntityTcpResolver>(getRegistry());
	}
}
