#include "../common/header.h"

#include "OptionsFileDescriptor.h"

#include <map>
#include <boost/variant.hpp>

namespace Omni {
	OptionsFileDescriptor::DefineT OptionsFileDescriptor::define = {
		{"cloexec", {Parser::Type::FLAG, FlagT([](type fd) {})}},
	};
}
