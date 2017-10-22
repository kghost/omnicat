#include "../common/header.h"

#include "Engine.h"

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#include "Module.h"

namespace Omni {
	std::shared_ptr<Module> getModule() {
#ifdef __linux__ 
#elif _WIN32
		throw std::string(__FUNCDNAME__);
#else
#endif
	}

	std::string getFunctionName() {
		try {
			getModule();
		} catch (const std::string & name) {
			return name;
		}
		OMNI_INTERNAL_ERROR;
	}
	static std::string function = getFunctionName();

	Engine::Engine() {}

	void Engine::loadModule(const std::string & name) {
#ifdef __linux__
		double (*cosine)(double);
		char *error;

		auto lib = ::dlopen(name.c_str(), RTLD_LAZY);
		if (lib == NULL) { throw ExceptionModuleNotFound(name, std::string(::dlerror())); }
		auto proc = reinterpret_cast<decltype(&getModule)>(::dlsym(lib, function.c_str()));
		auto module = proc();
		// ::dlclose(lib);
#elif _WIN32
		auto lib = ::LoadLibrary(convUTF8toUCS2(name).c_str());
		if (lib == NULL) {
			auto err = ::GetLastError();
			LPWSTR buff = NULL;
			::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, err, 0, (LPWSTR)&buff, 0, NULL);
			auto reason = convUCS2toUTF8(std::wstring(buff));
			::LocalFree(buff);
			throw ExceptionModuleNotFound(name, reason);
		}

		auto proc = reinterpret_cast<decltype(&getModule)>(::GetProcAddress(lib, function.c_str()));
		auto module = proc();
#else
#error
#endif
		for (auto entry : module->getClasses(shared_from_this())) {
			// XXX: check for already exists entry
			auto & i = classes[entry.first];
			i = entry.second;
		}
	}

	EXPORT std::shared_ptr<Engine> getEngine() {
		return std::make_shared<Engine>();
	}
}
