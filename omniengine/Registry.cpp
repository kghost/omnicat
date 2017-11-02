#include "../common/header.h"

#include "Registry.h"

#ifdef _WIN32
#include <boost/type_traits/remove_pointer.hpp>
#endif
#ifdef __linux__
#include <boost/type_traits/function_traits.hpp>
#endif

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#include <boost/filesystem.hpp>

#include "encoding.h"
#include "Module.h"
#include "Factory.h"

namespace Omni {
	Registry::Registry() {}
	Registry::~Registry() {}

	class RegistryImpl : public Registry, public std::enable_shared_from_this<RegistryImpl> {
	public:
		RegistryImpl() {}
		virtual std::shared_ptr<Factory> getFactory(const std::string & name);
		virtual std::shared_ptr<Parser::Object> createObject(const std::string & name);
		virtual std::shared_ptr<Entity> createEntity(const std::string & name);
		virtual void loadModule(const std::string & name);
	private:
		std::map<std::string, std::shared_ptr<Factory>> classes;
	};

	std::shared_ptr<Factory> RegistryImpl::getFactory(const std::string & name) {
		auto r = classes.find(name);
		if (r != classes.end()) {
			return r->second;
		} else {
			throw ExceptionFactoryNotFound(name);
		}
	}

	std::shared_ptr<Parser::Object> RegistryImpl::createObject(const std::string & name) {
		return getFactory(name)->createObject();
	}

	std::shared_ptr<Entity> RegistryImpl::createEntity(const std::string & name) {
		return getFactory(name)->createEntity();
	}

#ifdef __linux__
	template <class T> struct remove_noexcept{ typedef T type; };
#ifdef __cpp_noexcept_function_type
	template <class T, typename ...ARGS> struct remove_noexcept<T(ARGS...) noexcept>{ typedef T(type)(ARGS...); };
#endif
	// export the symbol so backtrace_symbols can retrieve the function name
	EXPORT std::shared_ptr<Module> getModule() {
		Dl_info info;
		::dladdr(reinterpret_cast<typename boost::function_traits<
			remove_noexcept<decltype(::dladdr)>::type
			>::arg1_type>(&getModule), &info);
		throw std::string(info.dli_sname);
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

	void RegistryImpl::loadModule(const std::string & name) {
		boost::filesystem::path m(name);
		m.make_preferred();
#ifdef _WIN32
		static const boost::filesystem::path dir = "./";
		static const std::string ext = ".dll";
#else
		static const boost::filesystem::path dir = PKGLIBDIR;
		static const std::string ext = LT_MODULE_EXT;
#endif
		if (!m.has_extension()) m += ext;
		if (!m.has_parent_path()) m = dir / m;

		static std::string fname = getFunctionName();
#ifdef __linux__
		auto lib = ::dlopen(m.c_str(), RTLD_LAZY);
		if (lib == NULL) { throw ExceptionModuleNotFound(m.string(), std::string(::dlerror())); }
		auto proc = reinterpret_cast<decltype(&getModule)>(::dlsym(lib, fname.c_str()));
#elif _WIN32
		auto lib = ::LoadLibrary(m.c_str());
		if (lib == NULL) {
			auto err = ::GetLastError();
			LPWSTR buff = NULL;
			::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, err, 0, (LPWSTR)&buff, 0, NULL);
			std::shared_ptr<boost::remove_pointer<decltype(buff)>::type> p(buff, &::LocalFree);
			throw ExceptionModuleNotFound(m.string(), toUTF8(std::wstring(buff)));
		}

		auto proc = reinterpret_cast<decltype(&getModule)>(::GetProcAddress(lib, fname.c_str()));
#else
#error
#endif
		if (proc == nullptr) throw ExceptionModuleNotFound(m.string(), "can't locate entry function");
		auto module = proc();

		for (auto entry : module->getFactories(shared_from_this())) {
			// XXX: check for already exists entry
			auto & i = classes[entry.first];
			i = entry.second;
		}
	}

	SHARED std::shared_ptr<Registry> getRegistry() {
		return std::make_shared<RegistryImpl>();
	}
}
