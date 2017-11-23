#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "Instance.h"

namespace Omni {
	class Entity;
	class EntityPipeline;
	class Instance;
	class InstancePipeline : public Instance, public std::enable_shared_from_this<InstancePipeline> {
	public:
		InstancePipeline(std::vector<std::shared_ptr<Instance>>&& components);
		virtual ~InstancePipeline();

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);
	private:
		std::shared_ptr<EntityPipeline> entity;
		std::vector<std::shared_ptr<Instance>> components;
	};
}
