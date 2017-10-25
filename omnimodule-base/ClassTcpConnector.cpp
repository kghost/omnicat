#include "../common/header.h"

#include "ClassTcpConnector.h"

#include "EntityFileRecvSend.h"

namespace Omni {
	std::shared_ptr<Entity> Omni::ClassTcpConnector::defaultHanlder() {
		return std::make_shared<EntityFileRecvSend>();
	}

	std::shared_ptr<Entity> ClassTcpConnector::createEntity(std::map<std::string, std::string> options, std::shared_ptr<Entity> handler) {
		return std::shared_ptr<Entity>();
	}
}
