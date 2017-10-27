#pragma once

#include <memory>

#include "../omniengine/Exception.h"
#include "../omniengine/Factory.h"

namespace Omni {
	namespace Parser {
		class Object;
	}
	class FactoryTcpConnector : public Factory {
	public:
		FactoryTcpConnector(std::shared_ptr<Registry> registry) : Factory(registry) {};
		virtual ~FactoryTcpConnector() {};

		virtual std::shared_ptr<Parser::Object> createObject() { return nullptr; };
	};
}
