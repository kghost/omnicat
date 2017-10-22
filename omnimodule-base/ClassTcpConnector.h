#pragma once

#include <memory>

#include "../omniengine/Exception.h"
#include "../omniengine/Class.h"

namespace Omni {
	class ClassTcpConnector : public Class {
	public:
		ClassTcpConnector(std::shared_ptr<Engine> engine) : Class(engine) {};
		virtual ~ClassTcpConnector() {};

		virtual std::shared_ptr<Entity> defaultHanlder();
		virtual std::shared_ptr<Entity> createEntity(std::map<std::string, std::string> options, std::shared_ptr<Entity> handler);
	};
}