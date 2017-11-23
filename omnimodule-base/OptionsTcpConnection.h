#pragma once

#include "../omniengine/encoding.h"
#include "../omniengine/Registry.h"
#include "../omniengine/Exception.h"
#include "OptionsTemplate.h"
#include "EntityTcpResolver.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsTcpConnection : public OptionsTemplate<OptionsTcpConnection<OwnerT>, OwnerT> {
	public:
		typedef OptionsTemplate<OptionsTcpConnection<OwnerT>, OwnerT> ParentT;
		const static typename ParentT::DefineT define;
	};

	template<typename OwnerT>
	const typename OptionsTcpConnection<OwnerT>::ParentT::DefineT OptionsTcpConnection<OwnerT>::define = {
		{
			"cork", {
				Parser::Type::FLAG, {"", "If set, don't send out partial frames. See TCP_CORK."},
				typename ParentT::FlagT([](OwnerT& owner) {
				})
			}
		},
		{
			"nodelay", {
				Parser::Type::FLAG, {"", "If  set,  disable the Nagle algorithm. See TCP_NODELAY."},
				typename ParentT::FlagT([](OwnerT& owner) {
				})
			}
		},
		{
			"keepcnt", {
				Parser::Type::STRING, {"count", "The maximum number of keepalive probes TCP should send before dropping the connection."},
				typename ParentT::StringT([](OwnerT& owner, const std::string & value) {
				})
			}
		},
		{
			"keepidle", {
				Parser::Type::STRING, {"seconds", "The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes, if the socket option SO_KEEPALIVE has been set on this socket."},
				typename ParentT::StringT([](OwnerT& owner, const std::string & value) {
				})
			}
		},
		{
			"keepintvl", {
				Parser::Type::STRING, {"seconds", "The time (in seconds) between individual keepalive probes."},
				typename ParentT::StringT([](OwnerT& owner, const std::string & value) {
				})
			}
		}
	};
}

