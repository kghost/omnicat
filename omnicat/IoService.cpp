#include "../common/header.h"

#include "IoService.h"

#include <list>
#include <utility>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "../omniengine/Entity.h"
#include "../omniengine/Instance.h"
#include "../omniengine/Exception.h"
#include "../omniengine/Weave.h"
#include "../omniengine/WeaverAdapterAsio.h"

namespace Omni {
	template<typename Iterator>
	static Fiber::Fiber stop_all(boost::asio::io_service & io, Iterator begin, Iterator end, Fiber::Continuation continuation) {
		if (begin == end) return continuation();
		else {
			auto& next = *begin;
			return next->stop(io, [&io, begin = ++begin, end, continuation = std::move(continuation)]() mutable {
				return stop_all(io, begin, end, std::move(continuation));
			});
		}
	}

	void start(std::shared_ptr<Entity> e) {
		boost::asio::io_service io;

		boost::asio::signal_set signals(io, SIGINT, SIGTERM);
		std::list<std::shared_ptr<Instance>> objs;

		Fiber::run([&](auto&& exit) {
			return Fiber::Asio::yield<int>([&](auto&& handler) {
				signals.async_wait(handler([&io, &objs, exit = std::move(exit)](int signal_number) mutable {
					return stop_all(io, objs.begin(), objs.end(), std::move(exit));
				}));
			});
		});

		Fiber::run([&](auto&& exit) {
			return e->createInstance(io, [&objs, &io, exit = std::move(exit)](std::shared_ptr<Instance> && o) mutable {
				objs.push_back(o);
				return o->start(io, std::move(exit));
			});
		});

		io.run();
	}
}
