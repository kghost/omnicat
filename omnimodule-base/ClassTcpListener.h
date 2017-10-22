#pragma once

#include "../omniengine/Exception.h"
#include "../omniengine/Class.h"

namespace Omni {
	class ClassTcpListener : public Class {
	public:
		ClassTcpListener(std::shared_ptr<Engine> engine) : Class(engine) {};
		virtual ~ClassTcpListener() {}

		virtual std::shared_ptr<Entity> defaultHanlder() { OMNI_INTERNAL_ERROR; };
		virtual std::shared_ptr<Entity> createEntity(std::map<std::string, std::string> options, std::shared_ptr<Entity> handler) { OMNI_INTERNAL_ERROR; };
	};
}
