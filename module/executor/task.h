#pragma once
#include "memory.h"
#include "system/coroutine.h"
#include <coroutine>
#include <intrin.h>

class promise final : public library::promise<promise> {
public:
	inline explicit promise(void) noexcept = default;
	inline explicit promise(promise const&) noexcept = delete;
	inline explicit promise(promise&&) noexcept = delete;
	inline auto operator=(promise const&) noexcept -> promise & = delete;
	inline auto operator=(promise&&) noexcept -> promise & = delete;
	inline ~promise(void) noexcept = default;

	inline auto initial_suspend(void) noexcept -> std::suspend_always {
		return std::suspend_always();
	}
};

using coroutine = library::coroutine<promise>;

using task = std::coroutine_handle<promise>;