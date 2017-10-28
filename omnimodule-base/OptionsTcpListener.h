#pragma once

#include "../omniengine/Registry.h"
#include "../omniengine/Exception.h"
#include "OptionsTemplate.h"
#include "EntityTcpResolver.h"

namespace Omni {
	template<typename OwnerT, typename SocketT>
	class OptionsTcpListener : public OptionsTemplate<OptionsTcpListener<OwnerT, SocketT>, SocketT> {
	public:
		OptionsTcpListener(OwnerT& owner) : owner(owner) {}
		DefineT define = {
			{
				"", {
					Parser::Type::RAW,{"[address]:port", "Equivalent to \"bind = ${address}:${port}\"", ":omnicat"},
					RawT([this](const std::string & value, int) -> SetT {
						auto ws = fromUTF8(value);
						auto i = ws.find_last_of(':');
						if (i == std::string::npos) throw ExceptionInvalidArgument("malformed listening address or unknown option: " + value);
						if (bool(owner.resolver)) throw ExceptionInvalidArgument("multiple listening address: " + value);
						auto address = toUTF8(ws.substr(0, i));
						auto service = toUTF8(ws.substr(i + 1));
						auto resolver = std::dynamic_pointer_cast<EntityTcpResolver>(owner.getRegistry()->createEntity("TCP-RESOLVER"));
						if (!bool(resolver)) OMNI_INTERNAL_ERROR;
						resolver->setOption("address", address);
						resolver->setOption("service", service);
						owner.resolver = resolver;
						return {};
					})
				}
			},
			{
				"bind", {
					Parser::Type::RAW,{"[address]:port", "Equivalent to \"resolver = RESOLVER{ address = ${address}, service = ${port} }\""},
					RawT([this](const std::string & value, int) -> SetT {
						return [](SocketT& sock) -> void {};
					})
				}
			},
			{
				"resolver", {
					Parser::Type::RAW,{
						"RESOLVER{ family = [ ipv6, ipv4 ], address = ???, service = ???, port = ??? }",
						"Specify a RESOLVER to resolve the address and the port. By default it query the DNS to retrieve the address and port.",
						"RESOLVER{ family = [ ipv6, ipv4 ], address = \"\", service = omnicat }",
					},
					RawT([this](const std::string & value, int) -> SetT {
						return [](SocketT& sock) -> void {};
					})
				}
			},
		};
	private:
		OwnerT& owner;
	};
}

