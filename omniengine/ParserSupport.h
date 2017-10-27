#pragma once

#include <memory>

#include "shared.h"

namespace Omni {
	class Registry;
	class Entity;
	namespace Parser {
		enum class Type { NONE, FLAG, STRING, LIST, GROUP, OBJECT };

		// [ option1 | option2 | option3 ]
		class SHARED List {
			public:
				SHARED_MEMBER virtual ~List() = 0;
				virtual bool isListOptional() = 0;
				virtual Type listOptionType() = 0;
		};

		// {flagoption,namedoption=type}
		class SHARED Group {
			public:
				SHARED_MEMBER virtual ~Group() = 0;
				virtual bool isGroupOptional() = 0;
				virtual Type groupOptionType(const std::string & key) = 0; // return NONE for unknown option

				virtual bool setOption(const std::string & key, const std::string & value) = 0;
		};

		// Object can associate to group or list, eg:
		//   ObjectFactory{group} or ObjectFactory[list]
		// iif the Object can be dynamic casted to group or list.
		class SHARED Object {
			public:
				SHARED_MEMBER virtual ~Object() = 0;

				virtual bool hasList() = 0;
				virtual std::shared_ptr<List> getList() = 0;
				virtual bool hasGroup() = 0;
				virtual std::shared_ptr<Group> getGroup() = 0;
		};

		std::shared_ptr<Object> createObject(std::shared_ptr<Registry> registry, const std::string & name);
	}
}
