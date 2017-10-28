#include "../common/header.h"

#include "EntityAcceptor.h"

#include <boost/asio.hpp>
#include <boost/fusion/include/iteration.hpp>
#include "../omniengine/Exception.h"

namespace Omni {
	void EntityAcceptor::createInstance(std::function<int()> callback) { OMNI_INTERNAL_ERROR; }
	void EntityAcceptor::passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) { OMNI_INTERNAL_ERROR; }

	Parser::Type EntityAcceptor::groupOptionType(const std::string & key) {
		auto r = boost::fusion::fold(options, std::optional<Parser::Type>(), [&key](std::optional<Parser::Type> last, auto & e) -> std::optional<Parser::Type> {
			if (bool(last)) return last;
			else return e.optionType(key);
		});
		return r.value_or(Parser::Type::NONE);
	}

	bool EntityAcceptor::setRawOption(const std::string & value) {
		return false;
	}

	bool EntityAcceptor::setOption(const std::string & key) {
		auto r = boost::fusion::fold(options, std::optional<bool>(), [&key](std::optional<bool> last, auto & e) -> std::optional<bool> {
			if (bool(last)) return last;
			else return e.setOption(key);
		});
		return r.value_or(false);
	}

	bool EntityAcceptor::setOption(const std::string & key, const std::string & value) {
		auto r = boost::fusion::fold(options, std::optional<bool>(), [&key, &value](std::optional<bool> last, auto & e) -> std::optional<bool> {
			if (bool(last)) return last;
			else return e.setOption(key, value);
		});
		return r.value_or(false);
	}
}
