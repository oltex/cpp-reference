#pragma once
#include "library/container/read_copy_update.h"
#include "library/pattern/singleton.h"
#include "editor.h"
#include "resource.h"
#include "object.h"
#include <variant>

namespace framework {
	class inspector : public library::singleton<inspector> {
		friend class library::singleton<inspector>;
		std::variant<std::monostate, library::rcu_pointer<object>> _pointer;

		explicit inspector(void) noexcept = default;
		explicit inspector(inspector const&) noexcept = delete;
		explicit inspector(inspector&&) noexcept = delete;
		auto operator=(inspector const&) noexcept -> inspector & = delete;
		auto operator=(inspector&&) noexcept -> inspector & = delete;
		~inspector(void) noexcept = default;
	public:
		void update(void) noexcept;
		void set(library::rcu_pointer<object> pointer) noexcept;
	};
}