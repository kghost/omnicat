#pragma once

#include <list>
#include <queue>
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
		std::list<std::shared_ptr<Entity>> pipes;
		std::queue<std::shared_ptr<InstancePipeline>> pending;
	};
}
