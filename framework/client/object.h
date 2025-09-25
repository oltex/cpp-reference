#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/intrusive/list.h"
#include "library/container/intrusive/pointer.h"
#include "component.h"

namespace framework {
	class object : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
		//object_weak_ptr _parent;
		//object_list _child;
		//library::unorder_map<library::string, framework::component*> _component;
		//library::unorder_map<library::string, library::vector<library::string>> _system;
	public:
		explicit object(void) noexcept;
		explicit object(object const& rhs) noexcept {
			//for (auto& component : rhs._component) {
			//	 component._second->get_type_id();
			//}
		};
		explicit object(object&&) noexcept = delete;
		auto operator=(object const&) noexcept -> object & = delete;
		auto operator=(object&&) noexcept -> object & = delete;
		~object(void) noexcept {
			//while (!_child.empty()) {
			//	object& child = _child.front();
			//	_child.erase(child);

			//	library::intrusive::share_pointer<object, 0> child_pointer;
			//	child_pointer.set(&child);
			//}
		};

		//template<typename type>
		//inline void add_component(library::string const& name) noexcept {
		//	auto component = implement::component_manager::create_component<type>();
		//	_component.emplace(name, component);
		//}

		template<size_t index>
		inline void destruct(void) noexcept {};
		template<>
		inline void destruct<0>(void) noexcept {
			library::destruct(*this);
		};
		template<size_t index>
		inline static void deallocate(object* pointer) noexcept {};
		template<>
		inline static void deallocate<0>(object* pointer) noexcept {
			library::singleton<library::pool<object>>::instance().deallocate(pointer);
		};
	};

	class scenes;
	class objects final : public library::singleton<objects> {
		friend class library::singleton<objects>;
		friend class scenes;

		library::pool<framework::object> _pool;
		explicit objects(void) noexcept = default;
		explicit objects(objects const&) noexcept = delete;
		explicit objects(objects&&) noexcept = delete;
		auto operator=(objects const&) noexcept -> objects & = delete;
		auto operator=(objects&&) noexcept -> objects & = delete;
		~objects(void) noexcept = default;

		template<typename... argument>
		inline auto create_object(argument&&... arg) noexcept {
			auto obj = _pool.allocate(std::forward<argument>(arg)...);
			return library::intrusive::share_pointer<object, 0>(obj);
		}
	public:
		//inline void regist_prototype(library::string const& name, framework::object_share_ptr& object) noexcept {
		//	auto clone = create_object(*object);
		//	_object_prototype.emplace(name, clone);
		//}
		//inline auto clone_prototype(library::string const& name, framework::object_share_ptr& parent) noexcept -> library::intrusive::share_pointer<framework::object, 0> {
		//	auto result = _object_prototype.find(name);
		//	return create_object(*result->_second);
		//}
	};
}