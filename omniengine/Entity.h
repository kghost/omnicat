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
			SHARED virtual bool isPassive() = 0;
			SHARED virtual void prepare();

			SHARED virtual void createInstance(std::function<int()> callback);
			SHARED virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback);
	};
}
