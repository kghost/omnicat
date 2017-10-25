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

		virtual std::map<std::string, std::tuple<Key&, std::function<boost::any(const Parser::Tree&)>>> supportedOptions() = 0;
		virtual std::shared_ptr<Entity> createEntity(std::map<std::reference_wrapper<Key>, boost::any> options) = 0;
	protected:
		std::shared_ptr<Engine> getEngine() { return engine; }
	private:
		std::shared_ptr<Engine> engine;
	};
}