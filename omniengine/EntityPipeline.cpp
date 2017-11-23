#include "../common/header.h"

#include "EntityPipeline.h"

#include <boost/format.hpp>

#include "InstancePipeline.h"

namespace Omni {
	EntityPipeline::EntityPipeline() {}
	EntityPipeline::~EntityPipeline() {}

	Fiber::Fiber EntityPipeline::createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete) {
		auto size = components.size();
		auto result = std::make_shared<std::vector<std::shared_ptr<Instance>>>(size);
		auto children = std::make_shared<std::vector<Fiber::Fiber>>(size);
		for (int i = 0; i < size; ++i) {
			(*children)[i] = Fiber::fork(boost::str(boost::format("pipeline assemble %p %d") % this % i), [me = this->shared_from_this(), &io, result, i](auto&& exit) {
				return me->components[i]->createInstance(io, [me, &io, result, i, exit = std::move(exit)](std::shared_ptr<Instance> instance) {
					(*result)[i] = instance;
					return exit();
				});
			});
		};
		return Fiber::join(children->begin(), children->end(), [children, result, complete = std::move(complete)] {
			return complete(std::make_shared<InstancePipeline>(std::move(*result)));
		});
	}
}

