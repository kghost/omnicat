#pragma once

#include <cassert>
#include <typeinfo>
#include <optional>
#include <functional>
#include <boost/any.hpp>

#include "Key.h"

namespace Omni {
	class Symbol {
	public:
		typedef std::function<boost::any()> RefT;
		typedef std::function<RefT()> BindT;
		void set(BindT binding);
		operator bool(); // test whether the symbol is bound
		RefT ref(); // request a ref to the symbol
	private:
		Key& key;
		std::type_info type;
		std::optional<BindT> binding;
	};
}
