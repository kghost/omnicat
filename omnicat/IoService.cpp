#include "../common/header.h"

#include "IoService.h"

#include <utility>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "../omniengine/Entity.h"
#include "../omniengine/Instance.h"
#include "../omniengine/Exception.h"

namespace Omni {
	void start(std::shared_ptr<Entity> e) {
		boost::asio::io_service io;

		e->createInstance(io, {
			[&io](std::shared_ptr<Instance> o) {
				o->start(io, {
					[]() {},
					[](std::exception_ptr eptr) {}
				});
			},
			[](std::exception_ptr eptr) {}
		});
	}
}