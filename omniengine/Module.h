#pragma once

#include <map>
#include <memory>

namespace Omni {
	class Engine;
	class Class;
	class Module {
	public:
		virtual ~Module() {};

		virtual std::map<std::string, std::shared_ptr<Class>> getClasses(std::shared_ptr<Engine> engine) = 0;
	};
}