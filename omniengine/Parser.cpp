#include "../common/header.h"

#include "Parser.h"

#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_lazy.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/phoenix/core/value.hpp>

namespace Omni {
	using namespace boost::spirit;

	template <typename Iterator>
	class gm : public qi::grammar<Iterator, std::string()> {
	public:
		gm() : base_type(str) {
			//cls = str >> qi::char_(':');// >> options;
			//options = eps | (option >> ',' >> options);
			//option = str | option_value;
			//option_value = eps | ('=' >> qi::lazy(boost::phoenix::val(qi::char_)));
			str = *qi::char_;
		}

		qi::rule<Iterator, std::string()> cls, clsname, options, option, option_value, str;
	};

	std::shared_ptr<Entity> Parser::Parser::parse(const std::string & input) {
		boost::u8_to_u32_iterator<std::string::const_iterator> begin(input.begin()), end(input.end());
		gm<decltype(begin)> g;
		std::string v;
		bool result = boost::spirit::qi::parse(begin, end, g, v);
		return nullptr;
	}
}
