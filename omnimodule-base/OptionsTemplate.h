#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <map>
#include <boost/variant.hpp>
#include <boost/core/noncopyable.hpp>

#include "../omniengine/ParserSupport.h"

namespace Omni {
	class OptionDescription {
	public:
		std::string format;
		std::string description;
		std::optional<std::string> defv;
	};

	template <typename OptionType, typename OwnerT>
	class OptionsTemplate : private boost::noncopyable {
	public:
		// options handler type, being called inside setOption
		typedef std::function<void(OwnerT& owner, const std::string &, int)> RawT; // add a unused int parameter to make it different from StringT
		typedef std::function<void(OwnerT& owner)> FlagT;
		typedef std::function<void(OwnerT& owner, const std::string &)> StringT;

		// options define type
		typedef std::map<
			std::string,
			std::tuple<Parser::Type, OptionDescription, boost::variant<RawT, FlagT, StringT>>
		> DefineT;

		static std::optional<Parser::Type> optionType(const std::string & key) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return {};
			else return std::get<0>(i->second);
		}

		static bool setRawOption(OwnerT& owner, const std::string & value) {
			auto i = OptionType::define.find("");
			if (i == OptionType::define.end()) return false;
			assert(std::get<0>(i->second) == Parser::Type::RAW);
			boost::get<typename OptionType::RawT>(std::get<2>(i->second))(owner, value, 0);
			return true;
		}

		static bool setOption(OwnerT& owner, const std::string & key) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return false;
			assert(std::get<0>(i->second) == Parser::Type::FLAG);
			boost::get<typename OptionType::FlagT>(std::get<2>(i->second))(owner);
			return true;
		}

		static bool setOption(OwnerT& owner, const std::string & key, const std::string & value) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return false;
			assert(std::get<0>(i->second) == Parser::Type::STRING);
			boost::get<typename OptionType::StringT>(std::get<2>(i->second))(owner, value);
			return true;
		}
	};
}
