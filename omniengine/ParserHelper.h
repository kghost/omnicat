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

			virtual void setRawOption(const std::string & value) { target.setRawOption(value); }
			virtual void setOption(const std::string & key) { target.setOption(key); }
			virtual void setOption(const std::string & key, const std::string & value) { target.setOption(key, value); }
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

		template<typename TargetT, typename HelperType>
		class EntityWrapper : public HelperType {
		public:
			explicit EntityWrapper(std::shared_ptr<TargetT> result) : result(result), HelperType(*result) {}

			std::shared_ptr<Entity> getResult() { return result; }
		protected:
			std::shared_ptr<TargetT> result;
		};

		template<typename TargetT, typename HelperType, typename ...Args>
		std::shared_ptr<EntityWrapper<TargetT, HelperType>> wrapEntity(Args && ... args) {
			return std::make_shared<EntityWrapper<TargetT, HelperType>>(std::make_shared<TargetT>(std::forward<Args>(args)...));
		}
	}
}
