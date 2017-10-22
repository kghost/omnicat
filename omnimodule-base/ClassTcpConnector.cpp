#include "../common/header.h"

#include "ClassTcpConnector.h"

#include "EntityFileRecvSend.h"

namespace Omni {
	std::shared_ptr<Entity> Omni::ClassTcpConnector::defaultHanlder() {
		return std::make_shared<EntityFileRecvSend>();
	}
}
