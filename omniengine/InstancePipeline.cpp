#include "../common/header.h"

#include "InstancePipeline.h"

#include "Entity.h"
#include "EntityPipeline.h"

namespace Omni {
	InstancePipeline::InstancePipeline(std::vector<std::shared_ptr<Instance>>&& components) : components(std::move(components)) {}
	InstancePipeline::~InstancePipeline() {}

	Fiber::Fiber InstancePipeline::start(boost::asio::io_service& io, Completion<> complete) {
		return complete();
	}

	Fiber::Fiber InstancePipeline::stop(boost::asio::io_service& io, Completion<> complete) {
		return complete();
	}
}
