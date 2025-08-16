#pragma once
#include "task.h"
#include "system/interlock.h"
#include "system/wait_on_address.h"
#include "container/lockfree/queue.h"

class queue final : public library::lockfree::queue<task> {
	using base = library::lockfree::queue<task>;
	unsigned long _size;
public:
	inline explicit queue(void) noexcept
		: _size(0), base() {
	};
	inline explicit queue(queue const&) noexcept = delete;
	inline explicit queue(queue&&) noexcept = delete;
	inline auto operator=(queue const&) noexcept -> queue & = delete;
	inline auto operator=(queue&&) noexcept -> queue & = delete;
	inline ~queue(void) noexcept = default;

	template<typename... argument>
	inline void emplace(argument&&... arg) noexcept {
		library::interlock_increment(_size);
		base::emplace(std::forward<argument>(arg)...);
		library::wake_by_address_single(_size);
	}
	inline auto pop(void) noexcept -> std::optional<task> {
		auto result = base::pop();
		if (result)
			library::interlock_decrement(_size);
		return result;
	}
	inline void wait(void) noexcept {
		unsigned long _compare = 0;
		library::wait_on_address(_size, _compare, INFINITE);
	}
};