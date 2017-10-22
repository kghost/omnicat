#pragma once

#include <memory>
#include <map>
#include <functional>
#include <boost/any.hpp>

#include "Exception.h"
#include "Key.h"

namespace Omni {
	class Instance;
	class Entity {
		virtual bool isPassive() = 0;

		virtual void createInstance(std::function<int()> callback) { OMNI_INTERNAL_ERROR; };
		virtual void passiveCreateInstance(std::map<Key, boost::any> hints, std::function<int()> callback) { OMNI_INTERNAL_ERROR; };
	};
}