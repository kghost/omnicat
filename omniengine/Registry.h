#pragma once

#include <string>
#include <memory>
#include <map>

#include "Exception.h"

#include "shared.h"

namespace Omni {
	namespace Parser {
		class Object;
	}
	class Factory;
	class Entity;
	class SHARED Registry : public std::enable_shared_from_this<Registry> {
	public:
		Registry(); // don't export constructor to prevent other module creating instance
		SHARED std::shared_ptr<Factory> getFactory(const std::string & name);
		SHARED std::shared_ptr<Parser::Object> createObject(const std::string & name);

		SHARED void loadModule(const std::string & name);
	private:
		std::map<std::string, std::shared_ptr<Factory>> classes;
	};

	extern SHARED std::shared_ptr<Registry> getRegistry();
}
