#pragma once

#include <list>
#include <set>
#include <memory>

#include "Entity.h"

namespace Omni {
	class Instance;
	class InstancePipeline;
	class EntityPipeline : public Entity {
	public:
		EntityPipeline();
		virtual ~EntityPipeline();

		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete) = 0;
	private:
		std::list<std::shared_ptr<Entity>> components;
		std::set<std::shared_ptr<InstancePipeline>> forming;
		std::set<std::shared_ptr<InstancePipeline>> running;
	};
}
