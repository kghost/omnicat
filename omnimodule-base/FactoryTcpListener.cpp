#include "../common/header.h"

#include "FactoryTcpListener.h"

#include "memory"

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Key.h"
#include "EntityAcceptor.h"

namespace Omni {
	std::shared_ptr<Parser::Object> FactoryTcpListener::createObject() {
		return nullptr;
	}
}
