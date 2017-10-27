#pragma once

#include <memory>
#include <map>
#include <functional>
#include <boost/any.hpp>

namespace Omni {
	namespace Parser {
		class Tree;
	}
	class Engine;
	class Entity;
	class Key;
	class Class {
	public:
		Class(std::shared_ptr<Engine> engine) : engine(engine) {};
		virtual ~Class() {};

		virtual std::shared_ptr<Entity> createEntity() { return nullptr;  };
	protected:
		std::shared_ptr<Engine> getEngine() { return engine; }
	private:
		std::shared_ptr<Engine> engine;
	};
}