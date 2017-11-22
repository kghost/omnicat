#pragma once

#include "OptionsTemplate.h"

namespace Omni {
	template<typename OwnerT>
	class OptionsListener : public OptionsTemplate<OptionsListener<OwnerT>, OwnerT> {
	public:
		typedef OptionsTemplate<OptionsListener<OwnerT>, OwnerT> ParentT;
		const static typename ParentT::DefineT define;
	};

	template<typename OwnerT>
	const typename OptionsListener<OwnerT>::ParentT::DefineT OptionsListener<OwnerT>::define = {
		{
			"fork", {
				Parser::Type::FLAG, {"", "Equivalent to max=0. Establish unlimited number of connections. (Note: it doesn't fork sub-processes)"},
				typename ParentT::FlagT([](OwnerT& owner) {
					owner.limit = 0;
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
				typename ParentT::StringT([](OwnerT& owner, const std::string & value) {
					owner.limit = std::stoi(value);
				})
			}
		},
	};
}
