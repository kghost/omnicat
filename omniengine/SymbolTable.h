#pragma once

#include <set>
#include <map>

#include "Key.h"
#include "Symbol.h"

namespace Omni {
	class SymbolScope {}; // bound to instance, runtime symbols

	class SymbolTable { // bound to entity, used for static check before start
	public:
		std::set<Key&> verify(); // throw exception if fails, return all referenced Keys
	private:
		std::map<Key&, Symbol::RefT> symbols;
	};
}

