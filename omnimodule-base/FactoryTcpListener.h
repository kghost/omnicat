#pragma once

#include <array>

#include "../omniengine/Exception.h"
#include "../omniengine/Factory.h"

namespace Omni {
	namespace Parser {
		class Object;
	}
	class FactoryTcpListener : public Factory {
	public:
		FactoryTcpListener(std::shared_ptr<Registry> registry) : Factory(registry) {};
		virtual ~FactoryTcpListener() {}

		virtual std::shared_ptr<Parser::Object> createObject();
	};
}
