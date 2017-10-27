#pragma once

#include "../omniengine/Entity.h"

namespace Omni {
	class EntityTcpConnector : public Entity {
		virtual bool isPassive() { return false; };
	};
}
