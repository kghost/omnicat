#pragma once

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "Weave.h"
#include "shared.h"

namespace Omni {
	class Registry;
	class Instance;
	class SHARED Entity : private boost::noncopyable {
		public:
			Entity(std::shared_ptr<Registry> registry);
			virtual ~Entity() = 0;
			std::shared_ptr<Registry> getRegistry();

			// last step before start, should:
			//   * finalize options
			//   * verify options
			//   * resolve symbols
			virtual void prepare() = 0;

			virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete) = 0;
	protected:
		//SymbolTable symbols;
	private:
		std::shared_ptr<Registry> registry;
	};
}
