#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/intrusive/list.h"
#include "library/container/intrusive/share_pointer.h"
#include "component.h"

namespace framework {
	class object;
	using object_share_ptr = library::intrusive::share_pointer<object, 0>;
	using object_weak_ptr = library::intrusive::weak_pointer<object, 0>;
	class object_list : public library::intrusive::list<object, 0> {
		using base = library::intrusive::list<object, 0>;
	public:
		using base::base;
		inline ~object_list(void) noexcept {
			for (auto begin = base::begin(), end = base::end(); begin != end;) {
				object_share_ptr object_ptr;
				object_ptr.set(&*begin);
				begin = base::erase(begin);
			}
		}

		inline void insert(object_share_ptr object) noexcept {
			base::push_back(*object);
			object.reset();
		}
		inline static void erare(object_share_ptr& object) noexcept {
			object_share_ptr object_ptr;
			object_ptr.set(&*object);
			base::erase(*object);
		}
	};
	class object : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
		//object* _parent;
		object_list _child;
		library::unorder_map<library::string, framework::component*> _component;
		library::unorder_map<library::string, library::vector<library::string>> _system;
	public:
		inline explicit object(void) noexcept = default;
		inline explicit object(object const& rhs) noexcept {
			//for (auto& component : rhs._component) {
			//	 component._second->get_type_id();
			//}
		};
		inline explicit object(object&&) noexcept = delete;
		inline auto operator=(object const&) noexcept -> object & = delete;
		inline auto operator=(object&&) noexcept -> object & = delete;
		inline ~object(void) noexcept {
			//while (!_child.empty()) {
			//	object& child = _child.front();
			//	_child.erase(child);

			//	library::intrusive::share_pointer<object, 0> child_pointer;
			//	child_pointer.set(&child);
			//}
		};

		//inline void add_child(object& child) noexcept {
		//	_child.push_back(child);
		//}
		template<typename type, typename... argument>
		inline void add_component(library::string const& name) noexcept {
			_component_manager.create_component<type>();
			_component.emplace(name, component);
		}
	};
}