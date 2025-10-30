#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/read_copy_update.h"
#include "library/system/guid.h"
#include "component.h"

namespace framework {
	class object : public library::rcu_base<object> {
	protected:
		library::guid _guid;
		library::string _name;
	public:
		library::unorder_multimap<library::string, library::rcu_pointer<component>> _component;

		explicit object(library::string_view name) noexcept;
		explicit object(object const& rhs) noexcept;
		explicit object(object&&) noexcept = delete;
		auto operator=(object const&) noexcept -> object & = delete;
		auto operator=(object&&) noexcept -> object & = delete;
		~object(void) noexcept = default;

		auto name(void) noexcept -> library::string&;
		void edit(void) noexcept;
		template<typename type, typename... argument>
		inline auto add_component(library::string_view name, argument&&... arg) noexcept -> library::rcu_pointer<type> {
			library::rcu_pointer<type> component;
			if constexpr (std::is_constructible_v<type, library::rcu_pointer<object>, argument...>)
				component = components::instance().create<type>(pointer(), std::forward<argument>(arg)...);
			else
				component = components::instance().create<type>(std::forward<argument>(arg)...);
			_component.emplace(name, component);
			return component;
		}
	};

	//class objectp {
	//	std::variant<library::guid, library::rcu_pointer<object>> _pointer;
	//public:
	//	explicit objectp(void) noexcept = default;
	//	explicit objectp(objectp const&) noexcept = delete;
	//	explicit objectp(objectp&&) noexcept = delete;
	//	auto operator=(objectp const&) noexcept -> objectp & = delete;
	//	auto operator=(objectp&&) noexcept -> objectp & = delete;
	//	~objectp(void) noexcept = default;
	//};

	class objects final : public library::singleton<objects> {
		friend class library::singleton<objects>;
		friend class scenes;
		friend class object;
		friend class menu;
		library::pool<framework::object> _pool;
		//library::unorder_map<library::string, library::intrusive::share_pointer<object, 0>> _prototype;

		explicit objects(void) noexcept = default;
		explicit objects(objects const&) noexcept = delete;
		explicit objects(objects&&) noexcept = delete;
		auto operator=(objects const&) noexcept -> objects & = delete;
		auto operator=(objects&&) noexcept -> objects & = delete;
		~objects(void) noexcept = default;

		template<typename... argument>
		auto allocate(argument&&... arg) noexcept -> library::rcu_pointer<object> {
			auto pointer = _pool.allocate(std::forward<argument>(arg)...);
			auto rcu_pointer = library::rcu_pointer<object>(pointer);
			return rcu_pointer;
		}
		void deallocate(library::rcu_pointer<object> pointer) noexcept;
	public:
		//void regist_prototype(library::string const& name, library::intrusive::share_pointer<object, 0>& object) noexcept;
		//auto find_prototype(library::string const& name) noexcept -> library::intrusive::share_pointer<object, 0>;
	};
}