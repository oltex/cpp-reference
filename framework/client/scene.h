#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/vector.h"
#include "object.h"
#include "system.h"

namespace framework {
	class scene {
		using size_type = unsigned int;
		library::unorder_map<library::wstring, framework::object*> _object;
		library::unorder_map<size_type, library::vector<framework::system_interface*>> _system;
	public:
		inline explicit scene(void) noexcept = default;
		inline explicit scene(scene const&) noexcept = delete;
		inline explicit scene(scene&&) noexcept = delete;
		inline auto operator=(scene const&) noexcept -> scene & = delete;
		inline auto operator=(scene&&) noexcept -> scene & = delete;
		inline ~scene(void) noexcept = default;
	};
}