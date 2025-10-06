#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/intrusive/pointer_list.h"
#include "component.h"

namespace framework {
	class object : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
		friend class library::intrusive::share_pointer<object, 0>;
		friend class library::intrusive::weak_pointer<object, 0>;
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

		library::intrusive::pointer_list<component, 0, 0> _component;
	public:
		explicit object(void) noexcept;
		explicit object(object const& rhs) noexcept;
		explicit object(object&&) noexcept = delete;
		auto operator=(object const&) noexcept -> object & = delete;
		auto operator=(object&&) noexcept -> object & = delete;
		~object(void) noexcept = default;

		template<typename type, typename... argument>
		inline auto add_component(library::string const& name, argument&&... arg) noexcept -> library::intrusive::share_pointer<type, 0> {
			auto component = components::instance().allocate_component<type>(std::forward<argument>(arg)...);
			_component.push_back(component);
			return component;
		}
	};

	class objects final : public library::singleton<objects> {
		friend class library::singleton<objects>;
		friend class scenes;
		friend class object;
		friend class menu;
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
		void deallocate_object(object* value) noexcept;
	public:
		void regist_prototype(library::string const& name, library::intrusive::share_pointer<object, 0>& object) noexcept;
		auto find_prototype(library::string const& name) noexcept -> library::intrusive::share_pointer<object, 0>;
	};

	template<>
	inline static void object::deallocate<0>(object* pointer) noexcept {
		objects::instance().deallocate_object(pointer);
	};
}