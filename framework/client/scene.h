#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "object.h"
#include "system.h"

namespace framework {
	class scene {
		library::unorder_map<library::wstring, framework::component*> _object;
	public:
		inline explicit scene(void) noexcept = default;
		inline explicit scene(scene const&) noexcept = delete;
		inline explicit scene(scene&&) noexcept = delete;
		inline auto operator=(scene const&) noexcept -> scene & = delete;
		inline auto operator=(scene&&) noexcept -> scene & = delete;
		inline ~scene(void) noexcept = default;
	};
}