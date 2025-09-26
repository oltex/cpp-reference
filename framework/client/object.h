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
		explicit object(object const& rhs) noexcept;
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
		inline static void deallocate<0>(object* pointer) noexcept;
	};

	class objects final : public library::singleton<objects> {
		friend class library::singleton<objects>;
		friend class scenes;
		friend class object;
		library::pool<framework::object, false> _pool;
		library::unorder_map<library::string, library::intrusive::share_pointer<object, 0>> _prototype;

		explicit objects(void) noexcept = default;
		explicit objects(objects const&) noexcept = delete;
		explicit objects(objects&&) noexcept = delete;
		auto operator=(objects const&) noexcept -> objects & = delete;
		auto operator=(objects&&) noexcept -> objects & = delete;
		~objects(void) noexcept = default;

		template<typename... argument>
		inline auto allocate_object(argument&&... arg) noexcept -> library::intrusive::share_pointer<object, 0> {
			auto pointer = _pool.allocate();
			library::construct<object>(*pointer, std::forward<argument>(arg)...);
			return library::intrusive::share_pointer<object, 0>(pointer);
		}
		inline auto deallocate_object(object* value) noexcept {
			_pool.deallocate(value);
		}
	public:
		inline void regist_prototype(library::string const& name, library::intrusive::share_pointer<object, 0>& object) noexcept {
			auto clone = allocate_object(*object);
			_prototype.emplace(name, clone);
		}
		inline void find_prototype() noexcept {

		}
		//inline auto clone_prototype(library::string const& name, framework::object_share_ptr& parent) noexcept -> library::intrusive::share_pointer<framework::object, 0> {
		//	auto result = _object_prototype.find(name);
		//	return create_object(*result->_second);
		//}
	};

	template<>
	inline static void object::deallocate<0>(object* pointer) noexcept {
		objects::instance().deallocate_object(pointer);
	};
}