#include "../common/header.h"

#include "ClassTcpListener.h"

#include "../omniengine/Key.h"

namespace Omni {
	static std::array<std::tuple<Key, int, int>, 10> options = {
		std::tuple<Key, int, int>("a", 1, 1),
	};

	//std::map<std::string, std::tuple<Key&, std::function<boost::any(const Parser::Tree&)>>> supportedOptions() {

	//}
}