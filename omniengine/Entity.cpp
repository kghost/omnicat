#include "../common/header.h"

#include "Entity.h"

#include "Exception.h"

namespace Omni {
	SHARED void Entity::createInstance(std::function<int()> callback) { OMNI_INTERNAL_ERROR; }
	SHARED void Entity::passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) { OMNI_INTERNAL_ERROR; }
}
