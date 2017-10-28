#pragma once

#include <string>
#include <boost/optional/optional.hpp>
#include <boost/core/noncopyable.hpp>

namespace Omni {
	class Key : private boost::noncopyable {
	public:
		Key() : symbol() {}
		Key(std::string symbol) : symbol(symbol) {}
		bool operator==(const Key & that) { return this == &that; }
		bool operator<(const Key & that) { return this < &that; }
	private:
		boost::optional<std::string> symbol;
	};
}
