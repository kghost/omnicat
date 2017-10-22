#pragma once

#include <memory>
#include <map>

namespace Omni {
	class Engine;
	class Entity;
	class Class {
	public:
		Class(std::shared_ptr<Engine> engine) : engine(engine) {};
		virtual ~Class() {};

		virtual std::shared_ptr<Entity> defaultHanlder() = 0;
		virtual std::shared_ptr<Entity> createEntity(std::map<std::string, std::string> options, std::shared_ptr<Entity> handler) = 0;

		std::shared_ptr<Entity> createEntity(std::map<std::string, std::string> options) {
			return createEntity(options, defaultHanlder());
		}
	protected:
		std::shared_ptr<Engine> getEngine() { return engine; }
	private:
		std::shared_ptr<Engine> engine;
	};
}