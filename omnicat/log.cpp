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

void log_init() {
	boost::log::add_common_attributes();

	auto core = boost::log::core::get();
	core->add_global_attribute("Component", boost::log::attributes::constant<std::string>("Global"));

	auto sink = boost::log::add_console_log(std::clog);
	sink->locked_backend()->auto_flush(true);

	boost::log::formatter logFmt = boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
		% boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		% boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
		% boost::log::trivial::severity
		% boost::log::expressions::attr<boost::log::attributes::constant<std::string>::value_type>("Component")
		% boost::log::expressions::smessage;

	sink->set_formatter(logFmt);
}
