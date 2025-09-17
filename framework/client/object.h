#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "component.h"

namespace framework {
	class object {
		library::unorder_map<library::string, framework::component*> _component;
	public:
		inline explicit object(void) noexcept = default;
		inline explicit object(object const&) noexcept = delete;
		inline explicit object(object&&) noexcept = delete;
		inline auto operator=(object const&) noexcept -> object & = delete;
		inline auto operator=(object&&) noexcept -> object & = delete;
		inline ~object(void) noexcept = default;

		inline void add_component(library::string& name, framework::component* component) noexcept {
			_component.emplace(name, component);
		}
	};
}