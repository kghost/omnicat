#pragma once

#include "../omniengine/Entity.h"

namespace Omni {
	class EntityTcpConnector : public Entity {
		virtual bool isPassive() { return false; };

		virtual void createInstance(std::function<int()> callback) { OMNI_INTERNAL_ERROR; };
		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) { OMNI_INTERNAL_ERROR; };
	};
}
