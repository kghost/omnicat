#include "../common/header.h"

#include "Parser.h"

#include <boost/any.hpp>

#include "utilities.h"
#define BOOST_SPIRIT_USE_PHOENIX_V3
#define BOOST_SPIRIT_UNICODE // We'll use unicode (UTF8) all throughout
#include <boost/regex/pending/unicode_iterator.hpp>
#ifdef _DEBUG
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

namespace Omni {
	namespace Parser {
		SHARED_DEFINE Parser::Parser(std::shared_ptr<Engine> engine) : engine(engine) {}

		std::string f(std::vector<std::vector<boost::uint32_t>> & v);

		template <typename Iterator>
		class gm : public boost::spirit::qi::grammar<Iterator, std::vector<boost::uint32_t>()> {
		public:
			gm() : gm::base_type(result) {
				boost::spirit::qi::debug(double_quoted_char);
				boost::spirit::qi::debug(single_quoted);
				boost::spirit::qi::debug(double_quoted);
				boost::spirit::qi::debug(str_elem);
				boost::spirit::qi::debug(str);
				boost::spirit::qi::debug(result);
			}

		private:
			boost::spirit::qi::rule<Iterator, boost::uint32_t()> double_quoted_char = {
				(~boost::spirit::unicode::char_("\"\\")) |
				((boost::spirit::lit('\\') >> boost::spirit::unicode::char_("\"\\"))),
				"double_quoted_char"
			};
			boost::spirit::qi::rule<Iterator, std::vector<boost::uint32_t>()> single_quoted = {
				boost::spirit::lit('\'') >> (*(~boost::spirit::unicode::char_('\''))) >> boost::spirit::lit('\''),
				"single_quoted"
			};
			boost::spirit::qi::rule<Iterator, std::vector<boost::uint32_t>()> double_quoted = {
				boost::spirit::lit('\"') >> (*double_quoted_char) >> boost::spirit::lit('\"'),
				"double_quoted"
			};
			boost::spirit::qi::rule<Iterator, std::vector<boost::uint32_t>()> str_elem = {
				(~boost::spirit::unicode::char_("{}()=,\'\"\\")) |
				(boost::spirit::lit('\\') >> boost::spirit::unicode::char_("{}()=,\'\"\\")) |
				double_quoted | single_quoted,
				"str_elem"
			};
			boost::spirit::qi::rule<Iterator, std::string()> str = {
				(*str_elem)[
					boost::spirit::_val = ""
				],
				"str"
			};
			//boost::spirit::qi::rule<Iterator, std::map<std::string, boost::any>()> options = {
			//	boost::spirit::lit(',') ,
			//	"options"
			//};
			//boost::spirit::qi::rule<Iterator, std::string()> cls_name = {
			//	str,
			//	"cls_name"
			//};
			//boost::spirit::qi::rule<Iterator, std::string()> cls = {
			//	cls_name >> boost::spirit::lit('{') >> options >> boost::spirit::lit('}')
			//	"cls"
			//};
			//boost::spirit::qi::rule<Iterator> clsname, options, option_value;
			boost::spirit::qi::rule<Iterator, std::vector<boost::uint32_t>()> result = {
				str >> boost::spirit::eoi,
				"result"
			};
		};

		SHARED_DEFINE std::shared_ptr<Entity> Parser::parse(const std::string & input) {
			boost::u8_to_u32_iterator<std::string::const_iterator> begin(input.begin()), end(input.end());
			std::string v;
			bool result = boost::spirit::qi::parse(begin, end, gm<decltype(begin)>(), v);
			return nullptr;
		}
	}
}
