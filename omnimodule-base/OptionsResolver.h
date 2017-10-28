#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsResolver: public OptionsTemplate<OptionsResolver<OwnerT>> {
	public:
		typedef OptionsTemplate<OptionsResolver<OwnerT>> ParentT;
		OptionsResolver(OwnerT& owner) : owner(owner) {}
		typename ParentT::DefineT define = {
			{
				"family", {
					Parser::Type::LIST,{"<address>", "The address to resolve", "localhost"},
					typename ParentT::FlagT([this]() -> typename ParentT::SetT {
						return {};
					})
				}
			},
			{
				"address", {
					Parser::Type::STRING, {"<address>", "The address to resolve", "localhost"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						owner.address = value;
						return {};
					})
				}
			},
			{
				"service", {
					Parser::Type::STRING, {"<string>", "The service to resolve", "omnicat"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						owner.service = value;
						return {};
					})
				}
			},
			{
				"port", {
					Parser::Type::STRING, {"<number>", "Manually set the port number, skip resolve service"},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						owner.port = std::stoi(value);
						return {};
					})
				}
			},
		};
	private:
		OwnerT& owner;
	};
}

