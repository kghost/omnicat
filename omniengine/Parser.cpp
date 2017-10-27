#include "../common/header.h"

#include "Parser.h"

#include <tuple>
#include <iostream>
#include <type_traits>
#include <optional>
#include <map>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#define BOOST_SPIRIT_UNICODE // We'll use unicode (UTF8) all throughout
#include <boost/regex/pending/unicode_iterator.hpp>
#ifdef _DEBUG
#include "utilities.h"
#define BOOST_SPIRIT_DEBUG
//#define BOOST_SPIRIT_DEBUG_OUT Omni::utf8cout
#include <boost/spirit/include/classic_debug.hpp>
#endif
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_lazy.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/phoenix/operator.hpp>

#include "ParserSupport.h"

namespace Omni {
	namespace Parser {
		template <size_t n, typename... T>
		typename std::enable_if<(n >= sizeof...(T))>::type
			print_tuple(std::ostream&, const std::tuple<T...>&) {}

		template <size_t n, typename... T>
		typename std::enable_if<(n < sizeof...(T))>::type
			print_tuple(std::ostream& os, const std::tuple<T...>& tup) {
			if (n != 0)
				os << ", ";
			os << std::get<n>(tup);
			print_tuple<n + 1>(os, tup);
		}

		template <typename... T>
		std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& tup) {
			os << "[";
			print_tuple<0>(os, tup);
			return os << "]";
		}

		template <typename Ostream, typename T>
		Ostream& operator<<(Ostream& os, const T & o) {
			os << "Obj@";
			return os;
		}

		class Placeholder {};
		class Options {};

		template <typename Iterator>
		class gm : public boost::spirit::qi::grammar<Iterator, std::shared_ptr<Object>()> {
		public:
			gm(std::shared_ptr<Registry> registry) : gm::base_type(result), registry(registry) {
				boost::spirit::qi::debug(str);
				boost::spirit::qi::debug(option_value);
				boost::spirit::qi::debug(object);
				boost::spirit::qi::debug(result);
			}

		private:
			std::shared_ptr<Registry> registry;

			boost::spirit::qi::rule<Iterator, wchar_t()> double_quoted_char = {
				(~boost::spirit::unicode::char_("\"\\")) |
				((boost::spirit::lit('\\') >> boost::spirit::unicode::char_("\"\\"))),
				"double_quoted_char"
			};
			boost::spirit::qi::rule<Iterator, std::vector<wchar_t>()> single_quoted = {
				boost::spirit::lit('\'') >> (*(~boost::spirit::unicode::char_('\''))) >> boost::spirit::lit('\''),
				"single_quoted"
			};
			boost::spirit::qi::rule<Iterator, std::vector<wchar_t>()> double_quoted = {
				boost::spirit::lit('\"') >> (*double_quoted_char) >> boost::spirit::lit('\"'),
				"double_quoted"
			};
			boost::spirit::qi::rule<Iterator, std::vector<wchar_t>()> str_elem = {
				(~boost::spirit::unicode::char_("{}[]()=,\'\"\\")) |
				(boost::spirit::lit('\\') >> boost::spirit::unicode::char_("{}[]()=,\'\"\\")) |
				double_quoted | single_quoted,
				"str_elem"
			};
			boost::spirit::qi::rule<Iterator, std::string()> str = {
				(*str_elem)[
					boost::spirit::_val = boost::phoenix::bind([](std::vector<std::vector<wchar_t>> & v) -> std::string {
						std::basic_stringstream<wchar_t> ss;
						std::ostream_iterator<wchar_t, wchar_t> wssi(ss);
						for (auto o : v) std::copy(o.begin(), o.end(), wssi);
						return toUTF8(ss.str());
					}, boost::spirit::_1)
				],
				"str"
			};
			boost::spirit::qi::rule<Iterator, Placeholder()> option_value = {
				str[
					boost::spirit::_val = boost::phoenix::bind([](const std::string & v) -> Placeholder {
						return Placeholder();
					}, boost::spirit::_1)
				],
				/*lazy(
				boost::phoenix::bind([]() {
				return boost::spirit::eps;
				}, boost::spirit::_1)
				)*/
						"option_value"
			};
			boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<std::string, std::string>> option(std::shared_ptr<Group> p1) {
				auto rule = boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<std::string, std::string>>{
					(str[boost::spirit::_a = boost::spirit::_1] >> -(boost::spirit::lit('=') >>
						lazy(
							boost::phoenix::bind([this, p1](const std::string & v) -> boost::spirit::qi::rule<Iterator, std::string()> {
								return str;
							}, boost::spirit::_a)
						)[boost::spirit::_b = boost::spirit::_1]
					))[
						boost::spirit::_pass = boost::phoenix::bind([this, p1](const std::string & key, const std::string & val) -> bool {
							return p1->setStringOption(key, val);
						}, boost::spirit::_a, boost::spirit::_b)
					],
					"option"
				};
				boost::spirit::qi::debug(rule);
				return rule;
			}
			boost::spirit::qi::rule<Iterator> list(std::shared_ptr<List> p1) {
				auto rule = boost::spirit::qi::rule<Iterator>{
					boost::spirit::lit('[') >> (str % boost::spirit::lit('|')) >> boost::spirit::lit(']'),
					"list"
				};
				boost::spirit::qi::debug(rule);
				return rule;
			};
			boost::spirit::qi::rule<Iterator> group(std::shared_ptr<Group> p1) {
				auto rule = boost::spirit::qi::rule<Iterator>{
					boost::spirit::lit('{') >> (option(p1) % boost::spirit::lit(',')) >> boost::spirit::lit('}'),
					"group"
				};
				boost::spirit::qi::debug(rule);
				return rule;
			}
			boost::spirit::qi::rule<Iterator, std::shared_ptr<Object>(), boost::spirit::qi::locals<std::shared_ptr<Object>>> object = {
				(str[
					boost::spirit::_pass = boost::phoenix::bind([this](const std::string & p1, std::shared_ptr<Object> a) -> bool {
						return bool(a = createObject(registry, p1));
					}, boost::spirit::_1, boost::spirit::_a)
				] >> lazy(
					boost::phoenix::bind([this](std::shared_ptr<Object> a) -> boost::spirit::qi::rule<Iterator> {
						if (a->hasGroup()) return group(a->getGroup());
						else if (a->hasList()) return list(a->getList());
						else return boost::spirit::eps;
					}, boost::spirit::_a)
				))[boost::spirit::_val = boost::spirit::_a],
				"object"
			};
			boost::spirit::qi::rule<Iterator, std::shared_ptr<Object>()> result = {
				object >> boost::spirit::eoi,
				"result"
			};
		};

		SHARED std::shared_ptr<Object> parse(std::shared_ptr<Registry> registry, const std::string & input) {
			boost::u8_to_u32_iterator<std::string::const_iterator> begin(input.begin()), end(input.end());
			std::shared_ptr<Object> v;
			if (boost::spirit::qi::parse(begin, end, gm<decltype(begin)>(registry), v)) {
				return v;
			} else {
				return nullptr;
			}
		}
	}
}
