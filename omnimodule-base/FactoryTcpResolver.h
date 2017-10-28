#pragma once

#include <array>

#include "../omniengine/Exception.h"
#include "../omniengine/Factory.h"

namespace Omni {
	namespace Parser {
		class Object;
	}
	class FactoryTcpResolver : public Factory {
	public:
		FactoryTcpResolver(std::shared_ptr<Registry> registry) : Factory(registry) {};
		virtual ~FactoryTcpResolver() {}

		virtual std::shared_ptr<Parser::Object> createObject();
		virtual std::shared_ptr<Entity> createEntity();
	};
}
