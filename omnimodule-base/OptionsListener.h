#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsListener : public OptionsTemplate<OptionsListener<OwnerT>> {
	public:
		typedef OptionsTemplate<OptionsListener<OwnerT>> ParentT;
		OptionsListener(OwnerT& owner) : owner(owner) {}
		typename ParentT::DefineT define = {
			{
				"fork", {
					Parser::Type::FLAG, {"", "Equivalent to max=0. Establish unlimited number of connections. (Note: it doesn't fork sub-processes)"},
					typename ParentT::FlagT([this]() -> typename ParentT::SetT {
						owner.limit = 0;
						return {};
					})
				}
			},
			{
				"max", {
					Parser::Type::STRING, {
						"<count>",
						"Limits the number of concurrent child processes [int]. Use 0 to disable.",
						"1",
					},
					typename ParentT::StringT([this](const std::string & value) -> typename ParentT::SetT {
						owner.limit = std::stoi(value);
						return {};
					})
				}
			},
		};
	private:
		OwnerT& owner;
	};
}

