#pragma once

#include "../omniengine/Entity.h"

namespace Omni {
	class EntityFileRecvSend : public Entity {
	public:
		virtual bool isPassive() { return true; };

		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) {

		};
	};
}
