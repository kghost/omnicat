﻿#include "../common/header.h"

#include <fcntl.h>
#ifdef USE_WIDECHAR_API
#include <io.h>
#endif
#include <stdio.h>

#include "../omniengine/Registry.h"
#include "../omniengine/Factory.h"
#include "../omniengine/Parser.h"

#ifdef USE_WIDECHAR_API
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int main(int argc, char *argv[], char *envp[])
#endif
{
#ifdef USE_WIDECHAR_API
	//::_setmode(_fileno(stdin), _O_U16TEXT);
	//::_setmode(_fileno(stdout), _O_U16TEXT);
	//::_setmode(_fileno(stderr), _O_U16TEXT);
#endif
	try {
		auto registry = Omni::getRegistry();
		registry->loadModule("omnimodule-base");
		auto p = Omni::Parser::parse(registry, toUTF8(
			L" TCP-LISTEN { fork, localhost:12345 ( a b )}"
		));
	} catch (const Omni::Exception & exception) {
#ifdef USE_WIDECHAR_API
		std::wcerr << fromUTF8(std::string(exception.what())) << std::endl;
#else
		std::cerr << exception.what() << std::endl;
#endif
	}
	return 0;
}
