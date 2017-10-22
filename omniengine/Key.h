#pragma once

#include <string>
#include <boost/optional/optional.hpp>

namespace Omni {
	class Key {
	public:
		Key() : symbol() {}
		Key(std::string symbol) : symbol(symbol) {}
		Key(const Key &) = delete;
		Key & operator=(const Key &) = delete;
	private:
		boost::optional<std::string> symbol;
	};
}

namespace std {
	template<> class less<Omni::Key> {
		bool operator() (const Omni::Key& lhs, const Omni::Key& rhs) const {
			return &lhs < &rhs;
		}
	};
}
