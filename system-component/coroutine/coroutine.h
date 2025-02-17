#pragma once
#include <coroutine>

namespace system_component {
	class coroutine final {
	private:
		class promise final {
			coroutine get_return_object(void) noexcept {
				return coroutine{ std::coroutine_handle<promise>::from_promise(*this) };
			}
			std::suspend_never initial_suspend(void) noexcept {
				return {};
			}
			std::suspend_never final_suspend() noexcept {
				return {};
			}
		};
	public:
		inline explicit coroutine(void) noexcept = default;
		inline explicit  coroutine(std::coroutine_handle<promise> handler) noexcept
			: _handle(handler) {
		}
		inline explicit coroutine(coroutine const&) noexcept = delete;
		inline explicit coroutine(coroutine&&) noexcept = delete;
		inline auto operator=(coroutine const&) noexcept -> coroutine & = delete;
		inline auto operator=(coroutine&&) noexcept -> coroutine & = delete;
		inline ~coroutine(void) noexcept {
		};

	private:
		std::coroutine_handle<promise> _handle;
	};
}