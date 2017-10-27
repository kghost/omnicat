#pragma once

#include <memory>

#include "shared.h"

namespace Omni {
	class Registry;
	namespace Parser {
		class Object;
		std::shared_ptr<Object> parse(std::shared_ptr<Registry> registry, const std::string & input);
	}
}
