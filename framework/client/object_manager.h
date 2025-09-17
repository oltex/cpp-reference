#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "object.h"

namespace framework {
	class object_manager {
		library::unorder_map<library::string, framework::object*> _object;
	public:
		inline explicit object_manager(void) noexcept = default;
		inline explicit object_manager(object_manager const&) noexcept = delete;
		inline explicit object_manager(object_manager&&) noexcept = delete;
		inline auto operator=(object_manager const&) noexcept -> object_manager & = delete;
		inline auto operator=(object_manager&&) noexcept -> object_manager & = delete;
		inline ~object_manager(void) noexcept = default;

		inline void regist_prototype(library::string& name, framework::object* object) noexcept {
			_object.emplace(name, object);
		}
		inline auto find_prototype(library::string& name) noexcept -> object* {
			auto result = _object.find(name);
			return result->_second;
		}
	};
}