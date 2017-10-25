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

		bool operator==(const Key & that) { return this == &that; }
		bool operator<(const Key & that) { return this < &that; }
	private:
		boost::optional<std::string> symbol;
	};
}
