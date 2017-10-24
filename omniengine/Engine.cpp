#include "../common/header.h"

#include "Engine.h"

#ifdef __linux__
#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#endif

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif

#include "Module.h"

namespace Omni {
#ifdef __linux__
	// export the symbol so backtrace_symbols can retrieve the function name
	EXPORT std::shared_ptr<Module> getModule() {
		std::array<
			boost::remove_pointer<
				boost::function_traits<decltype(::backtrace)>::arg1_type
			>::type,
			1
		> stacks;
		auto count = ::backtrace(stacks.data(), stacks.size());
		auto funcs = std::shared_ptr<
			boost::remove_pointer<
				boost::function_traits<decltype(::backtrace_symbols)>::result_type
			>::type
		>(::backtrace_symbols(stacks.data(), count), ::free);
		throw std::string(funcs.get()[0]);
	}
#elif _WIN32
	std::shared_ptr<Module> getModule() {
		throw std::string(__FUNCDNAME__);
	}
#else
#endif

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
