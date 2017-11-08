#include "../common/header.h"

#include <fcntl.h>
#ifdef USE_WIDECHAR_API
#include <io.h>
#endif
#include <stdio.h>
#include <boost/log/utility/setup/console.hpp>

#include "../omniengine/utilities.h"
#include "../omniengine/encoding.h"
#include "../omniengine/Registry.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Parser.h"
#include "../omniengine/ParserSupport.h"

#include "IoService.h"

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
#ifdef USE_WIDECHAR_API
		boost::log::add_console_log(Omni::utf8cerr);
#else
		boost::log::add_console_log(std::cout);
#endif
		auto registry = Omni::getRegistry();
		registry->loadModule("omnimodule-base");
		auto p = Omni::Parser::parse(registry, Omni::toUTF8(
			L" TCP-LISTEN { fork, localhost:12345 ( a b ) }"
		));
		auto e = p->getResult();
		e->prepare();
		start(e);
	} catch (const Omni::Exception & exception) {
#ifdef USE_WIDECHAR_API
		Omni::utf8cerr << exception.what() << std::endl;
#else
		std::cerr << exception.what() << std::endl;
#endif
	}
	return 0;
}
