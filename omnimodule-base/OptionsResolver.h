#pragma once

#include "OptionsTemplate.h"

#include "../omniengine/Exception.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsResolver: public OptionsTemplate<OptionsResolver<OwnerT>> {
	public:
		typedef OptionsTemplate<OptionsResolver<OwnerT>> ParentT;
		OptionsResolver(OwnerT& owner) : owner(owner) {}
		typename ParentT::DefineT define = {
			{
				"family", {
					Parser::Type::LIST,{"[ family2, family2 ]", "The address family to use", "[ ipv4, ipv6 ]"},
					typename ParentT::FlagT([this]() -> typename ParentT::SetT {
						return {};
					})
				}
			},
			{
				"host", {
					Parser::Type::STRING, {"<string>", "The host to resolve", "localhost"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						if (bool(owner.hasHost)) throw ExceptionInvalidArgument("host and address can't be set multiple times :" + value);
						owner.hasHost = false;
						owner.host = value;
						return {};
					})
				}
			},
			{
				"address",{
					Parser::Type::STRING,{"<address>", "Directly set address, skip resolve"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						if (bool(owner.hasHost)) throw ExceptionInvalidArgument("host and address can't be set multiple times :" + value);
						owner.hasHost = true;
						owner.host = value;
						return {};
					})
				}
			},
			{
				"service", {
					Parser::Type::STRING, {"<string>", "The service to resolve", "omnicat"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						if (bool(owner.hasService)) throw ExceptionInvalidArgument("service and port can't be set multiple times :" + value);
						owner.hasService = false;
						owner.service = value;
						return {};
					})
				}
			},
			{
				"port", {
					Parser::Type::STRING, {"<number>", "Directly set port number, skip resolve"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						if (bool(owner.hasService)) throw ExceptionInvalidArgument("service and port can't be set multiple times :" + value);
						owner.hasService = true;
						owner.service = value;
						return {};
					})
				}
			},
		};
	private:
		OwnerT& owner;
	};
}

