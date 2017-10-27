#pragma once

#include <memory>
#include <map>
#include <functional>
#include <boost/any.hpp>

#include "Key.h"

#include "shared.h"

namespace Omni {
	class Instance;
	class SHARED Entity {
		public:
			SHARED_MEMBER virtual bool isPassive() = 0;
			SHARED_MEMBER virtual void prepare() = 0;

			SHARED_MEMBER virtual void createInstance(std::function<int()> callback);
			SHARED_MEMBER virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback);
	};
}
