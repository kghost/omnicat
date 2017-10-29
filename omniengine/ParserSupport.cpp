#include "../common/header.h"

#include "ParserSupport.h"

#include "Registry.h"
#include "Entity.h"
#include "Exception.h"

namespace Omni {
	namespace Parser {
		SHARED List::~List() {}
		SHARED Group::~Group() {}
		SHARED Object::~Object() {}

		std::shared_ptr<Object> createObject(std::shared_ptr<Registry> registry, const std::string & name) {
			return registry->createObject(name);
		}
	}
}
