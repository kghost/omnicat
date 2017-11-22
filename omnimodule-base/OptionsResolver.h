#pragma once

#include "OptionsTemplate.h"

#include "../omniengine/Exception.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsResolver: public OptionsTemplate<OptionsResolver<OwnerT>, OwnerT> {
	public:
		typedef OptionsTemplate<OptionsResolver<OwnerT>, OwnerT> ParentT;
		const static typename ParentT::DefineT define;
	};

	template<typename OwnerT>
	const typename OptionsResolver<OwnerT>::ParentT::DefineT OptionsResolver<OwnerT>::define = {
		{
			"family", {
				Parser::Type::LIST,{"[ family2, family2 ]", "The address family to use", "[ ipv4, ipv6 ]"},
				typename ParentT::FlagT([](OwnerT &owner) {})
			}
		},
		{
			"host", {
				Parser::Type::STRING, {"<string>", "The host to resolve", "localhost"},
				typename ParentT::StringT([](OwnerT &owner, const std::string & value) {
					if (bool(owner.hasHost)) throw ExceptionInvalidArgument("host and address can't be set multiple times :" + value);
					owner.hasHost = false;
					owner.host = value;
				})
			}
		},
		{
			"address",{
				Parser::Type::STRING,{"<address>", "Directly set address, skip resolve"},
				typename ParentT::StringT([](OwnerT &owner, const std::string & value) {
					if (bool(owner.hasHost)) throw ExceptionInvalidArgument("host and address can't be set multiple times :" + value);
					owner.hasHost = true;
					owner.host = value;
				})
			}
		},
		{
			"service", {
				Parser::Type::STRING, {"<string>", "The service to resolve", "omnicat"},
				typename ParentT::StringT([](OwnerT &owner, const std::string & value) {
					if (bool(owner.hasService)) throw ExceptionInvalidArgument("service and port can't be set multiple times :" + value);
					owner.hasService = false;
					owner.service = value;
				})
			}
		},
		{
			"port", {
				Parser::Type::STRING, {"<number>", "Directly set port number, skip resolve"},
				typename ParentT::StringT([](OwnerT &owner, const std::string & value) {
					if (bool(owner.hasService)) throw ExceptionInvalidArgument("service and port can't be set multiple times :" + value);
					owner.hasService = true;
					owner.service = value;
				})
			}
		},
	};
}
