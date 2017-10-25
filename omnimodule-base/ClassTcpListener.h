#pragma once

#include <array>

#include "../omniengine/Exception.h"
#include "../omniengine/Class.h"

namespace Omni {
	class ClassTcpListener : public Class {
	public:
		ClassTcpListener(std::shared_ptr<Engine> engine) : Class(engine) {};
		virtual ~ClassTcpListener() {}

		virtual std::shared_ptr<Entity> createEntity(const Parser::Tree & tree) { OMNI_INTERNAL_ERROR; };
	};
}
