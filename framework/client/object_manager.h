#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "object.h"
#include "library/pattern/singleton.h"

namespace framework {
	class object_manager final {
		library::unorder_map<library::string, object_share_ptr> _object;
	public:
		inline explicit object_manager(void) noexcept = default;
		inline explicit object_manager(object_manager const&) noexcept = delete;
		inline explicit object_manager(object_manager&&) noexcept = delete;
		inline auto operator=(object_manager const&) noexcept -> object_manager & = delete;
		inline auto operator=(object_manager&&) noexcept -> object_manager & = delete;
		inline ~object_manager(void) noexcept = default;

		template<typename... argument>
		inline auto create_object(argument&&... arg) noexcept -> object_share_ptr {
			auto object = library::allocate<framework::object>();
			library::construct(*object, std::forward<argument>(arg)...);
			return object_share_ptr(object);
		}
		inline void regist_prototype(library::string const& name, object_share_ptr& object) noexcept {
			_object.emplace(name, object);
		}
		inline auto clone_prototype(library::string const& name) noexcept -> object_share_ptr {
			auto result = _object.find(name);
			return create_object(*result->_second);
		}
	};
}