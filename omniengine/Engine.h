#pragma once

#include <string>
#include <memory>
#include <map>

#include "Exception.h"

namespace Omni {
	class Class;
	class Engine : public std::enable_shared_from_this<Engine> {
	public:
		Engine(); // don't export constructor to prevent other module creating instance
		std::shared_ptr<Class> getClass(const std::string & name) {
			auto r = classes.find(name);
			if (r != classes.end()) {
				return r->second;
			} else {
				throw ExceptionClassNotFound(name);
			}
		}

		EXPORT void loadModule(const std::string & name);
	private:
		std::map<std::string, std::shared_ptr<Class>> classes;
	};

	EXPORT std::shared_ptr<Engine> getEngine();
}
