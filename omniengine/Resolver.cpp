#include "../common/header.h"

#include "Resolver.h"

#include <memory>

#include "InstanceResolver.h"

namespace Omni {
	void Resolver::createInstance(boost::asio::io_service & io, Completion<std::shared_ptr<Instance>> complete) {
		return createInstance(io, Completion<std::shared_ptr<InstanceResolver>>{
			[complete](std::shared_ptr<InstanceResolver> e) { complete.ok(e); },
			complete.fail
		});
	}
}
