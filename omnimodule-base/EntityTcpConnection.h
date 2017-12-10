#pragma once

#include <tuple>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "../omniengine/ParserSupport.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Weave.h"

namespace Omni {
	class InstanceTcpConnection;
	class EntityTcpConnection : public Entity, public std::enable_shared_from_this<EntityTcpConnection> {
	public:
		typedef InstanceTcpConnection InstanceType;
	public:
		// needed by parser
		Parser::Type groupOptionType(const std::string & key);
		void setRawOption(const std::string & value);
		void setOption(const std::string & key);
		void setOption(const std::string & key, const std::string & value);
	};
}
