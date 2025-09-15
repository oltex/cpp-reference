#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "component.h"

namespace framework {
	class component_manager final {
		library::unorder_map<library::wstring, framework::component*> _component;
	public:
		inline explicit component_manager(void) noexcept = default;
		inline explicit component_manager(component_manager const&) noexcept = delete;
		inline explicit component_manager(component_manager&&) noexcept = delete;
		inline auto operator=(component_manager const&) noexcept -> component_manager & = delete;
		inline auto operator=(component_manager&&) noexcept -> component_manager & = delete;
		inline ~component_manager(void) noexcept = default;

		inline void add_prototype(void) noexcept {

		}
	};
}