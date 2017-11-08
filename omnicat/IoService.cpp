#include "../common/header.h"

#include "IoService.h"

#include <utility>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "../omniengine/Entity.h"
#include "../omniengine/Instance.h"
#include "../omniengine/Exception.h"
#include "../omniengine/Weave.h"

namespace Omni {
	void start(std::shared_ptr<Entity> e) {
		boost::asio::io_service io;

		Fiber::run([e, &io](auto&& exit) {
			return e->createInstance(io, [&io, exit = std::move(exit)](std::shared_ptr<Instance> && o) mutable {
				return o->start(io, std::move(exit));
			});
		});

		io.run();
	}
}
