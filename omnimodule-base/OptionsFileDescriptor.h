#pragma once

#include <boost/asio/socket_base.hpp>

#include "../omniengine/ParserSupport.h"
#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT, typename SocketT>
	class OptionsFileDescriptor : public OptionsTemplate<OptionsFileDescriptor<OwnerT, SocketT>, SocketT> {
	public:
		OptionsFileDescriptor(OwnerT& owner) : owner(owner) {}
		DefineT define = {
#ifdef HAVE_FCNTL_H
			{"cloexec",{Parser::Type::FLAG, FlagT([this]() -> SetT {
				owner.??? // TODO: set open2 flag
				return {true, [](SocketT& sock) -> void {}};
			})}},
#endif
		};
	private:
		OwnerT& owner;
	};
}
