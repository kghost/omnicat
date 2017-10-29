#pragma once

#include "Entity.h"
#include "AsyncCall.h"
#include "shared.h"

namespace Omni {
	class InstanceResolver;
	class SHARED Resolver : public Entity {
	public:
		virtual void createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete);
		virtual void createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<InstanceResolver>> complete) = 0;
	};
}
