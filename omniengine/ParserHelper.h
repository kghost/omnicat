#pragma once

#include <memory>

#include "ParserSupport.h"
#include "Exception.h"

namespace Omni {
	class Registry;
	class Entity;
	namespace Parser {
		template<typename TargetT>
		class NoGroup {
		public:
			NoGroup() = delete;
			static constexpr bool value = false;
		};

		template<typename TargetT, bool Optional, bool Pipeline>
		class HelperGroup : public Group {
		public:
			HelperGroup(TargetT & target) : target(target) {}
			virtual ~HelperGroup() {}
			static constexpr bool value = true;

			virtual bool isGroupOptional() { return Optional; }
			virtual Type groupOptionType(const std::string & key) {
				return target.groupOptionType(key);
			}

			virtual bool hasPipeline() { return Pipeline; }

			virtual bool setRawOption(const std::string & value) { return target.setRawOption(value); }
			virtual bool setOption(const std::string & key) { return target.setOption(key); }
			virtual bool setOption(const std::string & key, const std::string & value) { return target.setOption(key, value); }
		private:
			TargetT & target;
		};

		template<typename TargetT>
		class NoList : public List {
		public:
			NoList(TargetT&) { OMNI_INTERNAL_ERROR; }
			virtual ~NoList() {}
			static constexpr bool value = false;
			virtual bool isListOptional() { OMNI_INTERNAL_ERROR; }
			virtual Type listOptionType() { OMNI_INTERNAL_ERROR; }
		};

		template<typename TargetT, typename GroupT = NoGroup<TargetT>, typename ListT = NoList<TargetT>>
		class HelperObject : public Object {
		public:
			static_assert(!(ListT::value && GroupT::value));
			HelperObject(TargetT & target) : target(target) {}
			virtual ~HelperObject() {}
			virtual bool hasList() { return ListT::value; }
			virtual std::shared_ptr<List> getList() { return std::make_shared<ListT>(target); }
			virtual bool hasGroup() { return GroupT::value; }
			virtual std::shared_ptr<Group> getGroup() { return std::make_shared<GroupT>(target); }
		private:
			TargetT & target;
		};
	}
}
