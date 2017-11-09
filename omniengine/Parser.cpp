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
#ifndef NDEBUG
#define BOOST_SPIRIT_DEBUG
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

#include "encoding.h"
#include "ParserSupport.h"

namespace Omni {
	namespace Parser {
		template <typename Ostream>
		Ostream& operator<<(Ostream& os, const char * o) {
			os << std::string(o);
			return os;
		}

		template <typename Ostream, typename T>
		Ostream& operator<<(Ostream& os, T * o) {
			os << typeid(*o).name() << "#" << reinterpret_cast<std::uintptr_t>(o);
			return os;
		}

		template <typename Ostream, typename T>
		Ostream& operator<<(Ostream& os, T & o) {
			os << typeid(o).name() << "@" << reinterpret_cast<std::uintptr_t>(&o);
			return os;
		}

		class Placeholder {};

		class grammar : public boost::spirit::qi::grammar<
			boost::u8_to_u32_iterator<std::string::const_iterator>,
			std::shared_ptr<Object>(),
			boost::spirit::unicode::space_type
		> {
		public:
			typedef typename boost::u8_to_u32_iterator<std::string::const_iterator> Iterator;

			grammar(std::shared_ptr<Registry> registry) : grammar::base_type(result), registry(registry) {
#ifndef NDEBUG
				boost::spirit::qi::debug(str);
				boost::spirit::qi::debug(option_value);
				boost::spirit::qi::debug(option);
				boost::spirit::qi::debug(pipeline);
				boost::spirit::qi::debug(list);
				boost::spirit::qi::debug(group);
				boost::spirit::qi::debug(object);
				boost::spirit::qi::debug(result);
#endif
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
			boost::spirit::qi::rule<Iterator, wchar_t()> special_charator = {
				boost::spirit::unicode::char_("{}[]()=,\'\"\\") | boost::spirit::unicode::space,
				"special_charator"
			};
			boost::spirit::qi::rule<Iterator, std::vector<wchar_t>()> str_elem = {
				(boost::spirit::unicode::char_ - special_charator) |
				(boost::spirit::lit('\\') >> special_charator) |
				double_quoted | single_quoted,
				"str_elem"
			};
			boost::spirit::qi::rule<Iterator, std::string()> str = {
				boost::spirit::lexeme[
					(+str_elem)[
						boost::spirit::_val = boost::phoenix::bind([](std::vector<std::vector<wchar_t>> & v) -> std::string {
							std::basic_stringstream<wchar_t> ss;
							std::ostream_iterator<wchar_t, wchar_t> wssi(ss);
							for (auto o : v) std::copy(o.begin(), o.end(), wssi);
							return toUTF8(ss.str());
						}, boost::spirit::_1)
					]
				],
				"str"
			};
			boost::spirit::qi::rule<Iterator, void(std::shared_ptr<Group>, std::string), boost::spirit::unicode::space_type> option_value = {
				boost::spirit::lit('=') >> lazy(
					boost::phoenix::bind([this](std::shared_ptr<Group> r1, const std::string & r2) -> boost::spirit::qi::rule<Iterator> {
						switch (r1->groupOptionType(r2)) {
							case Type::STRING:
								return str[
									boost::phoenix::bind([r1, r2](const std::string & val) -> void {
										r1->setOption(r2, val);
									}, boost::spirit::_1)
								];
							default: return !boost::spirit::eps;
						}
					}, boost::spirit::_r1, boost::spirit::_r2)
				),
				"option_value"
			};
			boost::spirit::qi::rule<Iterator, void(std::shared_ptr<Group>), boost::spirit::qi::locals<std::string>, boost::spirit::unicode::space_type> option = {
				str[boost::spirit::_a = boost::spirit::_1] >> lazy(
					boost::phoenix::bind([this](std::shared_ptr<Group> r1, const std::string & a) -> boost::spirit::qi::rule<Iterator, boost::spirit::unicode::space_type> {
						switch (r1->groupOptionType(a)) {
							case Type::FLAG:
								r1->setOption(a);
								return boost::spirit::eps;
							case Type::GROUP:
							case Type::LIST:
							case Type::OBJECT:
							case Type::STRING:
								return option_value(boost::phoenix::val(r1), boost::phoenix::val(a));
							case Type::RAW:
								r1->setRawOption(a);
								return boost::spirit::eps;
							default:
								return !boost::spirit::eps;
						}
					}, boost::spirit::_r1, boost::spirit::_a)
				),
				"option"
			};
			boost::spirit::qi::rule<Iterator, void(std::shared_ptr<Group>), boost::spirit::unicode::space_type> pipeline = {
				(-boost::spirit::lit(',')) >> boost::spirit::lit('(') >> *str >> boost::spirit::lit(')'),
				"pipeline"
			};
			boost::spirit::qi::rule<Iterator, void(std::shared_ptr<List>), boost::spirit::unicode::space_type> list = {
				boost::spirit::lit('[') >> (str % boost::spirit::lit(',')) >> boost::spirit::lit(']'),
				"list"
			};
			boost::spirit::qi::rule<Iterator, void(std::shared_ptr<Group>), boost::spirit::unicode::space_type> group = {
				boost::spirit::lit('{') >> (option(boost::spirit::_r1) % boost::spirit::lit(',')) >> lazy(
					boost::phoenix::bind([this](std::shared_ptr<Group> r1) -> boost::spirit::qi::rule<Iterator, boost::spirit::unicode::space_type> {
						if (r1->hasPipeline()) return pipeline(boost::phoenix::val(r1));
						else return boost::spirit::eps;
					}, boost::spirit::_r1)
				)>> boost::spirit::lit('}'),
				"group"
			};
			boost::spirit::qi::rule<Iterator, std::shared_ptr<Object>(), boost::spirit::qi::locals<std::shared_ptr<Object>>,
				boost::spirit::unicode::space_type> object = {
				(str[
					boost::spirit::_pass = boost::phoenix::bind([this](const std::string & r1, std::shared_ptr<Object> & a) -> bool {
						return bool(a = createObject(registry, r1));
					}, boost::spirit::_1, boost::spirit::_a)
				] >> lazy(
					boost::phoenix::bind([this](std::shared_ptr<Object> a) -> boost::spirit::qi::rule<Iterator, void(), boost::spirit::unicode::space_type> {
						if (a->hasGroup()) {
							auto p = a->getGroup();
							if (p->isGroupOptional()) return group(boost::phoenix::val(p)) | boost::spirit::eps;
							else return group(boost::phoenix::val(p));
						} else if (a->hasList()) {
							auto p = a->getList();
							if (p->isListOptional()) return list(boost::phoenix::val(p)) | boost::spirit::eps;
							else return list(boost::phoenix::val(p));
						} else return boost::spirit::eps;
					}, boost::spirit::_a)
				))[boost::spirit::_val = boost::spirit::_a],
				"object"
			};
			boost::spirit::qi::rule<Iterator, std::shared_ptr<Object>(), boost::spirit::unicode::space_type> result = {
				object >> boost::spirit::eoi,
				"result"
			};
		};

		SHARED std::shared_ptr<Object> parse(std::shared_ptr<Registry> registry, const std::string & input) {
			boost::u8_to_u32_iterator<std::string::const_iterator> begin(input.begin()), end(input.end());
			std::shared_ptr<Object> v;
			if (boost::spirit::qi::phrase_parse(begin, end, grammar(registry), boost::spirit::unicode::space, v)) {
				return v;
			} else {
				return nullptr;
			}
		}
	}
}
