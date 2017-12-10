#include "../common/header.h"

#include "Entity.h"

#include "Exception.h"
#include "Registry.h"

namespace Omni {
	Entity::Entity(std::shared_ptr<Registry> registry) : registry(registry) {}
	Entity::~Entity() {}
	std::shared_ptr<Registry> Entity::getRegistry() {
		return registry;
	}
}
