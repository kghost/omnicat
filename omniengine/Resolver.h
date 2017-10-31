#pragma once

#include "Entity.h"
#include "Weave.h"
#include "shared.h"

namespace Omni {
	class InstanceResolver;
	class SHARED Resolver : public Entity {
	public:
		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete);
		virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<InstanceResolver>> complete) = 0;
	};
}
