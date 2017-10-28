#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT, typename SocketT>
	class OptionsSocketListener : public OptionsTemplate<OptionsSocketListener<OwnerT, SocketT>, SocketT> {
	public:
		OptionsSocketListener(OwnerT& owner) : owner(owner) {}
		DefineT define = {
		};
	private:
		OwnerT& owner;
	};
}
