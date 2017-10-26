#pragma once

#include <memory>

#include "shared.h"

namespace Omni {
	class Engine;
	class Entity;
	namespace Parser {
		class Parser {
		public:
			SHARED_DEFINE Parser(std::shared_ptr<Engine> engine);
			SHARED_DEFINE std::shared_ptr<Entity> Parser::Parser::parse(const std::string & input);
		private:
			std::shared_ptr<Engine> engine;
		};
	}
}