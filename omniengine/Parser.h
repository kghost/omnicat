#pragma once

#include <memory>

#include "shared.h"

namespace Omni {
	class Engine;
	class Entity;
	namespace Parser {
		class Parser {
		public:
			SHARED Parser(std::shared_ptr<Engine> engine);
			SHARED std::shared_ptr<Entity> parse(const std::string & input);
		private:
			std::shared_ptr<Engine> engine;
		};
	}
}
