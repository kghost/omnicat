#include "../common/header.h"

#include "Resolver.h"

#include <memory>

#include "InstanceResolver.h"

namespace Omni {
	SHARED_MEMBER Fiber::Fiber Resolver::createInstance(boost::asio::io_service & io, Completion<std::shared_ptr<Instance>> complete) {
		return createInstance(io, Completion<std::shared_ptr<InstanceResolver>>{
			[complete = std::move(complete)](std::shared_ptr<InstanceResolver> && e) { return complete(e); }
		});
	}
}
