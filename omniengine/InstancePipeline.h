#pragma once

#include <list>
#include <memory>
#include <optional>

#include "Instance.h"

namespace Omni {
	class Entity;
	class Instance;
	class InstancePipeline : public Instance {
	public:
		InstancePipeline();
		virtual ~InstancePipeline();
	private:
		std::list<std::tuple<std::shared_ptr<Entity>, std::optional<std::shared_ptr<Instance>>>> assembling;
	};
}
