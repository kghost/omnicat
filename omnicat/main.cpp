#include "../common/header.h"

#include <fcntl.h>
#ifdef USE_WIDECHAR_API
#include <io.h>
#endif
#include <stdio.h>
#include <boost/log/trivial.hpp>

#include "../omniengine/encoding.h"
#include "../omniengine/Registry.h"
#include "../omniengine/Entity.h"
#include "../omniengine/Parser.h"
#include "../omniengine/ParserSupport.h"

#include "IoService.h"
#include "log.h"

#ifdef USE_WIDECHAR_API
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int main(int argc, char *argv[], char *envp[])
#endif
{
	try {
		log_init();
		auto registry = Omni::getRegistry();
		registry->loadModule("omnimodule-base");
		auto p = Omni::Parser::parse(registry, Omni::toUTF8(
			L" TCP-LISTEN { fork, localhost:12345 ( a b ) }"
		));
		auto e = p->getResult();
		e->prepare();
		start(e);
		return 0;
	} catch (const Omni::Exception & exception) {
		BOOST_LOG_TRIVIAL(fatal) << exception.what();
		return -1;
	}
}
