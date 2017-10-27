#pragma once

#include <map>
#include <memory>

namespace Omni {
	class Registry;
	class Factory;
	class Module {
	public:
		virtual ~Module() {};

		virtual std::map<std::string, std::shared_ptr<Factory>> getFactories(std::shared_ptr<Registry> registry) = 0;
	};
}
