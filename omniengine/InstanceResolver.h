#pragma once

#include <tuple>
#include <typeinfo>
#include <vector>
#include <boost/any.hpp>

#include "Instance.h"
#include "Weave.h"
#include "shared.h"

namespace Omni {
	class SHARED InstanceResolver : public Instance {
	public:
		InstanceResolver();
		virtual ~InstanceResolver();

		typedef std::tuple<std::type_info, boost::any> EndpointT;
		virtual Fiber::Fiber resolve(boost::asio::io_service & io, bool passive, Completion<std::vector<EndpointT>&&> complete) = 0;

		virtual Fiber::Fiber start(boost::asio::io_service& io, Completion<> complete);
		virtual Fiber::Fiber stop(boost::asio::io_service& io, Completion<> complete);
	};
}
