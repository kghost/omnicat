#include "../common/header.h"

#include <memory>

#include "../omniengine/Module.h"
#include "ClassTcpConnector.h"
#include "ClassTcpListener.h"

namespace Omni {
	class BaseModule : public Module {
	public:
		virtual std::map<std::string, std::shared_ptr<Class>> getClasses(std::shared_ptr<Engine> engine) {
			return {
				{"TCP", std::make_shared<ClassTcpConnector>(engine)},
				{ "TCP-LISTEN", std::make_shared<ClassTcpListener>(engine) }
			};
		}
	};

	EXPORT std::shared_ptr<Module> getModule() {
		return std::make_shared<BaseModule>();
	}
}