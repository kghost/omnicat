#pragma once

#include <vector>
#include <set>
#include <memory>

#include "Entity.h"

namespace Omni {
	class Instance;
	class InstancePipeline;
	class EntityPipeline : public Entity, public std::enable_shared_from_this<EntityPipeline> {
	public:
		EntityPipeline(std::shared_ptr<Registry> registry);
		virtual ~EntityPipeline();

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete);
		friend class InstancePipeline;
	private:
		std::vector<std::shared_ptr<Entity>> components;
		std::set<std::shared_ptr<InstancePipeline>> forming;
		std::set<std::shared_ptr<InstancePipeline>> running;
	};
}
