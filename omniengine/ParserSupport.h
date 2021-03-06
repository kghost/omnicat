#pragma once

#include <string>
#include <memory>
#include <boost/core/noncopyable.hpp>

#include "shared.h"

namespace Omni {
	class Registry;
	class Entity;
	namespace Parser {
		enum class Type {
			NONE, // will fail the parser
			FLAG,
			STRING,
			LIST,
			GROUP,
			OBJECT,
			RAW
		};

		// [ option1 | option2 | option3 ]
		class SHARED List : private boost::noncopyable {
			public:
				SHARED_MEMBER virtual ~List() = 0;
				virtual bool isListOptional() = 0;
				virtual Type listOptionType() = 0;
		};

		// {flagoption,namedoption=type,(pipline1 | pipeline2)}
		class SHARED Group : private boost::noncopyable {
			public:
				SHARED_MEMBER virtual ~Group() = 0;
				virtual bool isGroupOptional() = 0;
				virtual Type groupOptionType(const std::string & key) = 0; // return NONE for unknown option

				virtual bool hasPipeline() = 0; // pipeline always has type of Object

				virtual void setRawOption(const std::string & value) = 0;
				virtual void setOption(const std::string & key) = 0;
				virtual void setOption(const std::string & key, const std::string & value) = 0;
		};

		// Object can associate to group or list, eg:
		//   ObjectFactory{group} or ObjectFactory[list]
		class SHARED Object : private boost::noncopyable {
			public:
				SHARED_MEMBER virtual ~Object() = 0;

				virtual std::shared_ptr<Entity> getResult() = 0;

				virtual bool hasList() = 0;
				virtual std::shared_ptr<List> getList() = 0;
				virtual bool hasGroup() = 0;
				virtual std::shared_ptr<Group> getGroup() = 0;
		};

		std::shared_ptr<Object> createObject(std::shared_ptr<Registry> registry, const std::string & name);
	}
}
