#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsListener : public OptionsTemplate<OptionsListener<OwnerT>> {
	public:
		OptionsListener(OwnerT& owner) : owner(owner) {}
		DefineT define = {
			{
				"fork", {
					Parser::Type::FLAG, {"", "Equivalent to max=0. Establish unlimited number of connections. (Note: it doesn't fork sub-processes)"},
					FlagT([this]() -> SetT {
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
					StringT([this](const std::string & value) -> SetT {
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

