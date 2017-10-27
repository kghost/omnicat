#include "../common/header.h"

#include "ClassTcpListener.h"

#include "memory"

#include "../omniengine/Key.h"
#include "EntityAcceptor.h"

namespace Omni {
	std::shared_ptr<Entity> ClassTcpListener::createEntity() {
		return std::make_shared<EntityAcceptor>();
	}
}
