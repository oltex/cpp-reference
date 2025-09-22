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
		object_weak_ptr _parent;
		object_list _child;
		library::unorder_map<library::string, framework::component*> _component;
		library::unorder_map<library::string, library::vector<library::string>> _system;
	public:
		explicit object(object_share_ptr& parent) noexcept;
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
		template<typename type>
		inline void add_component(library::string const& name) noexcept {
			auto component = implement::component_manager::create_component<type>();
			_component.emplace(name, component);
		}
	};
}

namespace implement {
	class object_manager final {
		library::pool<framework::object> _object_pool;
		library::unorder_map<library::string, library::intrusive::share_pointer<framework::object, 0>> _object_prototype;
	public:
		inline explicit object_manager(void) noexcept = default;
		inline explicit object_manager(object_manager const&) noexcept = delete;
		inline explicit object_manager(object_manager&&) noexcept = delete;
		inline auto operator=(object_manager const&) noexcept -> object_manager & = delete;
		inline auto operator=(object_manager&&) noexcept -> object_manager & = delete;
		inline ~object_manager(void) noexcept = default;

		template<typename... argument>
		inline auto create_object(argument&&... arg) noexcept -> framework::object_share_ptr {
			auto object = _object_pool.allocate(std::forward<argument>(arg)...);
			return object_share_ptr(object);
		}
		inline void regist_prototype(library::string const& name, framework::object_share_ptr& object) noexcept {
			auto clone = create_object(*object);
			_object_prototype.emplace(name, clone);
		}
		inline auto clone_prototype(library::string const& name, framework::object_share_ptr& parent) noexcept -> library::intrusive::share_pointer<framework::object, 0> {
			auto result = _object_prototype.find(name);
			return create_object(*result->_second);
		}
	};
}