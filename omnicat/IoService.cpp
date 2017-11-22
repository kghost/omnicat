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
	static Fiber::Fiber stop_all(boost::asio::io_service & io, Iterator begin, Iterator end, Fiber::ContinuationRef continuation) {
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

		Fiber::run([&](auto&& exit) {
			return e->createInstance(io, [&io, exit = std::move(exit)](std::shared_ptr<Instance> && o) mutable {
				return o->start(io, [&io, o, exit = std::move(exit)]() mutable {
					auto signals = std::make_shared<boost::asio::signal_set>(io, SIGINT, SIGTERM);
					return Fiber::Asio::yield<int>([&](auto&& handler) {
						signals->async_wait(handler([&io, signals, o, exit = std::move(exit)](int signal_number) mutable {
							return o->stop(io, std::move(exit));
						}));
					});
				});
			});
		});

		io.run();
	}
}
