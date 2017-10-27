#include "../common/header.h"

#include <memory>

#include "../omniengine/Module.h"
#include "FactoryTcpConnector.h"
#include "FactoryTcpListener.h"

namespace Omni {
	class BaseModule : public Module {
	public:
		virtual std::map<std::string, std::shared_ptr<Factory>> getFactories(std::shared_ptr<Registry> registry) {
			return {
				{"TCP", std::make_shared<FactoryTcpConnector>(registry)},
				{"TCP-LISTEN", std::make_shared<FactoryTcpListener>(registry)}
			};
		}
	};

	EXPORT std::shared_ptr<Module> getModule() {
		return std::make_shared<BaseModule>();
	}
}
