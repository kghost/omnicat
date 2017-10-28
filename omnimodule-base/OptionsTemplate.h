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
	template <typename Base, typename ...TargetT>
	class OptionsTemplateImpl : Base {
	public:
		template<typename ...Args>
		OptionsTemplateImpl(Args && ... args) : Base(std::forward<Args>(args)...) {}

		std::optional<Parser::Type> optionType(const std::string & key) {
			auto i = Base::define.find(key);
			if (i == Base::define.end()) return {};
			else return std::get<0>(i->second);
		}

		bool setRawOption(const std::string & value) {
			auto i = Base::define.find("");
			if (i == Base::define.end()) return false;
			assert(std::get<0>(i->second) == Parser::Type::RAW);
			std::optional<typename Base::DelayedT> delayed = boost::get<typename Base::RawT>(std::get<2>(i->second))(value, 0);
			if (delayed) options.push_back(delayed.value());
			return true;
		}

		bool setOption(const std::string & key) {
			auto i = Base::define.find(key);
			if (i == Base::define.end()) return {};
			assert(std::get<0>(i->second) == Parser::Type::FLAG);
			std::optional<typename Base::DelayedT> delayed = boost::get<typename Base::FlagT>(std::get<2>(i->second))();
			if (delayed) options.push_back(delayed.value());
			return true;
		}

		bool setOption(const std::string & key, const std::string & value) {
			auto i = Base::define.find(key);
			if (i == Base::define.end()) return false;
			assert(std::get<0>(i->second) == Parser::Type::STRING);
			std::optional<typename Base::DelayedT> delayed = boost::get<typename Base::StringT>(std::get<2>(i->second))(value);
			if (delayed) options.push_back(delayed.value());
			return true;
		}

		void execute(TargetT && ... target) {
			for (auto o : options) o(std::forward<TargetT>(target)...);
		}
	protected:
		std::vector<typename Base::DelayedT> options;
	};

	class OptionDescription {
	public:
		std::string format;
		std::string description;
		std::optional<std::string> defv;
	};

	template <typename OptionType, typename ...TargetT>
	class OptionsTemplate : private boost::noncopyable {
	public:
		typedef std::function<void(TargetT&...)> DelayedT; // delayed call, being called inside execute
		typedef std::optional<DelayedT> SetT; // return type of setOption

		// options handler type, being called inside setOption
		typedef std::function<SetT(const std::string &, int)> RawT; // add a unused int parameter to make it different from StringT
		typedef std::function<SetT()> FlagT;
		typedef std::function<SetT(const std::string &)> StringT;

		// options define type
		typedef std::map<
			std::string,
			std::tuple<Parser::Type, OptionDescription, boost::variant<RawT, FlagT, StringT>>
		> DefineT;

		typedef OptionsTemplateImpl<OptionType, TargetT...> Impl;
	};
}
