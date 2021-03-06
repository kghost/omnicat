#include "../common/header.h"

#include "EntityTcpListener.h"

#include <boost/asio.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/iteration.hpp>

#include "../omniengine/Exception.h"
#include "InstanceTcpListener.h"
#include "OptionsTcpListener.h"
#include "OptionsListener.h"

namespace Omni {
	void EntityTcpListener::prepare() { resolver->prepare();  }

	Fiber::Fiber EntityTcpListener::createInstance(boost::asio::io_service & io, Completion<std::shared_ptr<Instance>> complete) {
		return complete(std::make_shared<InstanceTcpListener>(shared_from_this(), io));
	}

	static std::tuple<
		OptionsTcpListener<EntityTcpListener>,
		OptionsListener<EntityTcpListener>
	> options;

	Parser::Type EntityTcpListener::groupOptionType(const std::string & key) {
		return boost::fusion::fold(options, std::optional<Parser::Type>(), [&key](std::optional<Parser::Type> last, auto & e) -> std::optional<Parser::Type> {
			if (bool(last)) return last;
			else return e.optionType(key);
		}).value_or(
			// no one support this option, search for raw option which has an empty key
			boost::fusion::fold(options, std::optional<Parser::Type>(), [](std::optional<Parser::Type> last, auto & e) -> std::optional<Parser::Type> {
				if (bool(last)) return last;
				else return e.optionType("");
			}).value_or(Parser::Type::NONE)
		);
	}

	void EntityTcpListener::setRawOption(const std::string & value) {
		auto r = boost::fusion::fold(options, false, [this, &value](bool last, auto & e) -> bool {
			if (last) return last;
			else return e.setRawOption(*this, value);
		});
	}

	void EntityTcpListener::setOption(const std::string & key) {
		auto r = boost::fusion::fold(options, false, [this, &key](bool last, auto & e) -> bool {
			if (last) return last;
			else return e.setOption(*this, key);
		});
		assert(r);
	}

	void EntityTcpListener::setOption(const std::string & key, const std::string & value) {
		auto r = boost::fusion::fold(options, false, [this, &key, &value](bool last, auto & e) -> bool {
			if (last) return last;
			else return e.setOption(*this, key, value);
		});
		assert(r);
	}
}
