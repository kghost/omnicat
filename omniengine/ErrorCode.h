#pragma once

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace Omni {
	namespace ErrorCode {
		typedef boost::system::error_code error_code;
		typedef boost::system::error_category error_category;
		typedef boost::system::error_condition error_condition;

		typedef boost::system::system_error system_error;

		using boost::system::system_category;
	}
}
