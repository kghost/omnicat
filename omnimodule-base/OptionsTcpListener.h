#pragma once

#include "../omniengine/encoding.h"
#include "../omniengine/Registry.h"
#include "../omniengine/Exception.h"
#include "OptionsTemplate.h"
#include "EntityTcpResolver.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsTcpListener : public OptionsTemplate<OptionsTcpListener<OwnerT>, OwnerT> {
	public:
		typedef OptionsTemplate<OptionsTcpListener<OwnerT>, OwnerT> ParentT;
		const static typename ParentT::DefineT define;
	};

	template<typename OwnerT>
	const typename OptionsTcpListener<OwnerT>::ParentT::DefineT OptionsTcpListener<OwnerT>::define = {
		{
			"", {
				Parser::Type::RAW, {"[address]:port", "Equivalent to \"bind = ${address}:${port}\"", ":omnicat"},
				typename ParentT::RawT([](OwnerT& owner, const std::string & value, int) {
					auto ws = fromUTF8(value);
					auto i = ws.find_last_of(':');
					if (i == std::string::npos) throw ExceptionInvalidArgument("malformed listening address or unknown option: " + value);
					if (bool(owner.resolver)) throw ExceptionInvalidArgument("multiple listening address: " + value);
					auto address = toUTF8(ws.substr(0, i));
					auto service = toUTF8(ws.substr(i + 1));
					auto resolver = std::dynamic_pointer_cast<EntityTcpResolver>(owner.getRegistry()->createEntity("TCP-RESOLVER"));
					assert(resolver);
					resolver->setOption("host", address);
					resolver->setOption("service", service);
					owner.resolver = resolver;
				})
			}
		},
		{
			"ref",{
				Parser::Type::STRING, {"symbol", "Bind the accepted connection instance to the symbol for future usage."},
				typename ParentT::StringT([](OwnerT& owner, const std::string & value) {
				})
			}
		},
		{
			"bind", {
				Parser::Type::STRING, {"[address]:port", "Equivalent to \"resolver = RESOLVER{ address = ${address}, service = ${port} }\""},
				typename ParentT::RawT([](OwnerT& owner, const std::string & value, int) {
				})
			}
		},
		{
			"resolver", {
				Parser::Type::STRING, {
					"RESOLVER{ family = [ ipv6, ipv4 ], address = ???, service = ???, port = ??? }",
					"Specify a RESOLVER to resolve the address and the port. By default it query the DNS to retrieve the address and port.",
					"RESOLVER{ family = [ ipv6, ipv4 ], address = \"\", service = omnicat }",
				},
				typename ParentT::RawT([](OwnerT& owner, const std::string & value, int) {
				})
			}
		},
	};
}

