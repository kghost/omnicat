#pragma once

#include <memory>
#include <map>
#include <functional>
#include <boost/any.hpp>
#include <boost/core/noncopyable.hpp>

#include "Key.h"

#include "shared.h"

namespace Omni {
	class Instance;
	class SHARED Entity : private boost::noncopyable {
		public:
			virtual ~Entity() = 0;

			virtual bool isPassive() = 0;
			virtual void prepare() = 0;

			virtual void createInstance(std::function<int()> callback) = 0;
			virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) = 0;
	};
}
