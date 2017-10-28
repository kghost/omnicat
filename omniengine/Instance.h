#pragma once

#include <boost/core/noncopyable.hpp>

namespace Omni {
	class Instance : private boost::noncopyable {
	public:
		virtual ~Instance() {}
	};
}
