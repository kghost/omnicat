#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsResolver: public OptionsTemplate<OptionsResolver<OwnerT>> {
	public:
		OptionsResolver(OwnerT& owner) : owner(owner) {}
		DefineT define = {
			{
				"family", {
					Parser::Type::LIST,{"<address>", "The address to resolve", "localhost"},
					FlagT([this]() -> SetT {
						return {};
					})
				}
			},
			{
				"address", {
					Parser::Type::STRING, {"<address>", "The address to resolve", "localhost"},
					StringT([this](const std::string & value) -> SetT {
						owner.address = value;
						return {};
					})
				}
			},
			{
				"service", {
					Parser::Type::STRING, {"<string>", "The service to resolve", "omnicat"},
					StringT([this](const std::string & value) -> SetT {
						owner.service = value;
						return {};
					})
				}
			},
			{
				"port", {
					Parser::Type::STRING, {"<number>", "Manually set the port number, skip resolve service"},
					StringT([this](const std::string & value) -> SetT {
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

