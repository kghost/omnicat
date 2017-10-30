#pragma once

#include <memory>
#include <boost/core/noncopyable.hpp>

#include "Weave.h"
#include "shared.h"

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace Omni {
	class Instance;
	class SHARED Entity : private boost::noncopyable {
		public:
			virtual ~Entity() = 0;

			// last step before start, should:
			//   * finalize options
			//   * verify options
			//   * resolve symbols
			virtual void prepare() = 0;

			virtual void createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete) = 0;
	protected:
		//SymbolTable symbols;
	};
}
