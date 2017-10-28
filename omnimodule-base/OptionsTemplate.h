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
	template <typename OptionType, typename TargetT>
	class OptionsTemplate : private boost::noncopyable {
	//private:
	//	BOOST_MOVABLE_BUT_NOT_COPYABLE(OptionsTemplate)
	public:
		typedef typename OptionsTemplate<OptionType, TargetT> base_type;
		typedef typename TargetT type;

		typedef std::function<void(TargetT&)> FlagT;
		typedef std::function<std::function<void(TargetT&)>(const std::string &)> StringT;
		typedef std::map<std::string, std::tuple<Parser::Type, boost::variant<FlagT, StringT>>> DefineT;

		std::optional<Parser::Type> optionType(const std::string & key) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return {};
			else return std::get<0>(i->second);
		}

		std::optional<bool> setOption(const std::string & key) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return {};
			assert(std::get<0>(i->second) == Parser::Type::FLAG);
			options.push_back(boost::get<FlagT>(std::get<1>(i->second)));
			return true;
		}

		std::optional<bool> setOption(const std::string & key, const std::string & value) {
			auto i = OptionType::define.find(key);
			if (i == OptionType::define.end()) return {};
			assert(std::get<0>(i->second) == Parser::Type::STRING);
			auto f = boost::get<StringT>(std::get<1>(i->second))(value);
			if (!bool(f)) return false;
			options.push_back(f);
			return true;
		}

		void execute(TargetT& target) {
			for (auto o : options) o(target);
		}
	protected:
		std::vector<std::function<void(TargetT&)>> options;
	};
}

