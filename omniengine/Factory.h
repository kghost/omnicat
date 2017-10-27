#pragma once

#include <memory>
#include <map>
#include <functional>
#include <boost/any.hpp>

namespace Omni {
	namespace Parser {
		class Object;
	}
	class Registry;
	class Entity;
	class Key;
	class Factory {
	public:
		Factory(std::shared_ptr<Registry> registry) : registry(registry) {};
		virtual ~Factory() {};

		virtual std::shared_ptr<Parser::Object> createObject() = 0;
	protected:
		std::shared_ptr<Registry> getRegistry() { return registry; }
	private:
		std::shared_ptr<Registry> registry;
	};
}
