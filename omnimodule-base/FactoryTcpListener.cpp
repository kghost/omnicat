#include "../common/header.h"

#include "FactoryTcpListener.h"

#include "memory"

#include "../omniengine/ParserHelper.h"
#include "EntityAcceptor.h"

namespace Omni {
	typedef Parser::HelperObject<EntityAcceptor, Parser::HelperGroup<EntityAcceptor, true, true>> ObjectAccepptorBase;
	class ObjectAccepptor : public ObjectAccepptorBase {
	public:
		ObjectAccepptor(std::shared_ptr<EntityAcceptor> p) : ObjectAccepptorBase(*p.get()), e(p) {}
	private:
		std::shared_ptr<EntityAcceptor> e;
	};

	std::shared_ptr<Parser::Object> FactoryTcpListener::createObject() {
		return std::make_shared<ObjectAccepptor>(std::make_shared<EntityAcceptor>());
	}
}
