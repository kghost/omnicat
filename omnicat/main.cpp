#include "../common/header.h"

#include <fcntl.h>
#ifdef USE_WIDECHAR_API
#include <io.h>
#endif
#include <stdio.h>

#include "../omniengine/Engine.h"
#include "../omniengine/Class.h"

#ifdef USE_WIDECHAR_API
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int main(int argc, char *argv[], char *envp[])
#endif
{
#ifdef USE_WIDECHAR_API
	::_setmode(_fileno(stdin), _O_U16TEXT);
	::_setmode(_fileno(stdout), _O_U16TEXT);
	::_setmode(_fileno(stderr), _O_U16TEXT);
#endif
	try {
		auto engine = Omni::getEngine();
		engine->loadModule("omnimodule-base.dll");
		engine->getClass("TCP")->createEntity(std::map<std::string, std::string>());
	} catch (const Omni::Exception & exception) {
#ifdef USE_WIDECHAR_API
		std::wcerr << convUTF8toUCS2(std::string(exception.what())) << std::endl;
#else
		std::cerr << exception.what() << std::endl;
#endif
	}
	return 0;
}
