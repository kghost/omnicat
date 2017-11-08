#include "../common/header.h"

#include "log.h"

#include <boost/log/attributes/constant.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "../omniengine/utilities.h"

void log_init() {
	boost::log::add_common_attributes();

	auto core = boost::log::core::get();
	core->add_global_attribute("Scope", boost::log::attributes::named_scope());
	core->add_global_attribute("Component", boost::log::attributes::constant<std::string>("Global"));

	auto sink =
#ifdef USE_WIDECHAR_API
	boost::log::add_console_log(Omni::utf8cerr);
#else
	boost::log::add_console_log(std::clog);
#endif
	auto fmtTimeStamp = boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
	auto fmtThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
	auto fmtSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity");
	auto fmtComponent = boost::log::expressions::attr<boost::log::attributes::constant<std::string>::value_type>("Component");
	auto fmtScope = boost::log::expressions::format_named_scope("Scope",
		boost::log::keywords::format = "%n(%f:%l)",
		boost::log::keywords::iteration = boost::log::expressions::reverse,
		boost::log::keywords::depth = 2);
	boost::log::formatter logFmt = boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%/%5%] %6%") % fmtTimeStamp % fmtThreadId % fmtSeverity % fmtComponent % fmtScope % boost::log::expressions::smessage;

	sink->set_formatter(logFmt);
}
