#pragma once

#include <string>
#include <memory>
#include <map>
#include <boost/core/noncopyable.hpp>

#include "Exception.h"

#include "shared.h"

namespace Omni {
	namespace Parser {
		class Object;
	}
	class Factory;
	class Entity;
	class SHARED Registry : private boost::noncopyable {
	public:
		virtual ~Registry() = 0;
		virtual std::shared_ptr<Factory> getFactory(const std::string & name) = 0;
		virtual std::shared_ptr<Parser::Object> createObject(const std::string & name) = 0;
		virtual std::shared_ptr<Entity> createEntity(const std::string & name) = 0;
		virtual void loadModule(const std::string & name) = 0;
	};

	extern SHARED std::shared_ptr<Registry> getRegistry();
}
