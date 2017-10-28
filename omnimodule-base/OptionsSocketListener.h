#pragma once

#ifdef _WIN32
#include <Winsock2.h>
#endif

#include "../omniengine/ParserSupport.h"
#include "OptionsTemplate.h"

namespace Omni {
	class OptionsSocketListener : public OptionsTemplate<OptionsSocketListener,
#ifdef _WIN32
		SOCKET
#else
		int
#endif // _WIN
	> {
	public:
		void execute(type fd);

		static DefineT define;
	};
}
