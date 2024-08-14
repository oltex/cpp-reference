#pragma once
#include "design-pattern/singleton.h"

namespace engine {
	class component_manager final : public singleton<component_manager> {
		friend class singleton<component_manager>;
	private:
		inline explicit component_manager(void) noexcept = default;
		inline ~component_manager(void) noexcept {
		};
		inline explicit component_manager(component_manager const& rhs) noexcept = delete;
		inline auto operator=(component_manager const& rhs) noexcept -> component_manager & = delete;
		inline explicit component_manager(component_manager&& rhs) noexcept = delete;
		inline auto operator=(component_manager&& rhs) noexcept -> component_manager & = delete;
	private:

	};
}