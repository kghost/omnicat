#pragma once

#include <memory>

#include "../omniengine/Exception.h"
#include "../omniengine/Class.h"

namespace Omni {
	class ClassTcpConnector : public Class {
	public:
		ClassTcpConnector(std::shared_ptr<Engine> engine) : Class(engine) {};
		virtual ~ClassTcpConnector() {};

		virtual std::shared_ptr<Entity> createEntity() { return nullptr; };
	};
}