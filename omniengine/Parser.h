#pragma once

#include <memory>

namespace Omni {
	class Engine;
	class Entity;
	namespace Parser {
		class Parser {
		public:
			Parser(std::shared_ptr<Engine> engine) : engine(engine) {}
			std::shared_ptr<Entity> Parser::Parser::parse(const std::string & input);
		private:
			std::shared_ptr<Engine> engine;
		};
	}
}