#pragma once

#include "../omniengine/ParserSupport.h"
#include "OptionsTemplate.h"

namespace Omni {
	class OptionsFileDescriptor : public OptionsTemplate < OptionsFileDescriptor,
#ifdef _WIN32
		HANDLE
#else
		int
#endif // _WIN
	> {
	public:
		void execute(type fd);

		static DefineT define;
	};
}

