#pragma once

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "Weave.h"
#include "shared.h"

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

			virtual Fiber::Fiber createInstance(boost::asio::io_service& io, Completion<std::shared_ptr<Instance>> complete) = 0;
	protected:
		//SymbolTable symbols;
	};
}
