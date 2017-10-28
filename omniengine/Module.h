#pragma once

#include <map>
#include <memory>
#include <boost/core/noncopyable.hpp>

namespace Omni {
	class Registry;
	class Factory;
	class Module : private boost::noncopyable {
	public:
		virtual ~Module() {};

		virtual std::map<std::string, std::shared_ptr<Factory>> getFactories(std::shared_ptr<Registry> registry) = 0;
	};
}
