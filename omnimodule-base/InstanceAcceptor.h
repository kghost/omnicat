#pragma once

#include <boost/asio.hpp>

#include "../omniengine/Instance.h"

namespace Omni {
	class InstanceAcceptor : public Instance {
	public:
		virtual ~InstanceAcceptor();

		void read();
	private:
		boost::asio::ip::tcp::socket socket;
	};
}
