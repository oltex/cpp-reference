#pragma once
#include <coroutine>
#include <iostream>
#include <Windows.h>

namespace system_component {
	class coroutine final {
	public:
		class promise final {
		public:
			class suspend final {
			public:
				inline bool await_ready(void) noexcept {
					printf("await ready\n");
					return false;
				}
				inline void await_suspend(std::coroutine_handle<void> handle) noexcept {
					printf("await suspend\n");
					Sleep(1000);
				}
				inline int await_resume(void) noexcept {
					printf("await resume\n");
					return 10;
				}
			};
		public:
			inline explicit promise(void) noexcept = default;
			inline explicit promise(promise const&) noexcept = delete;
			inline explicit promise(promise&&) noexcept = delete;
			inline auto operator=(promise const&) noexcept -> promise & = delete;
			inline auto operator=(promise&&) noexcept -> promise & = delete;
			inline ~promise(void) noexcept = default;

			inline auto get_return_object(void) noexcept -> coroutine {
				return coroutine(std::coroutine_handle<promise>::from_promise(*this));
			}
			inline auto initial_suspend(void) noexcept -> std::suspend_always {
				return std::suspend_always();
			}
			inline auto final_suspend(void) noexcept -> std::suspend_always {
				return std::suspend_always();
			}

			inline auto yield_value(int result) noexcept -> std::suspend_always {
				printf("yield value\n");
				return std::suspend_always();
			}
			inline auto await_transform(int result) noexcept -> suspend {
				printf("await transform\n");
				return suspend();
			}

			inline void return_void(void) noexcept {
				printf("return void\n");
			}
			//inline void return_value(int result) noexcept {
			//	printf("return value\n");
			//}
			inline void unhandled_exception(void) noexcept {
				printf("unhandled exception\n");
			}
		};
		using promise_type = promise;
	public:
		inline explicit coroutine(std::coroutine_handle<promise> handler) noexcept
			: _handle(handler) {
		}
		inline explicit  coroutine(coroutine const& rhs) noexcept = delete;
		inline coroutine(coroutine&& rhs) noexcept
			: _handle(rhs._handle) {
			rhs._handle = nullptr;
		};
		inline auto operator=(coroutine const&) noexcept -> coroutine & = delete;
		inline auto operator=(coroutine&& rhs) noexcept -> coroutine& {
			_handle = rhs._handle;
			rhs._handle = nullptr;
		};
		inline ~coroutine(void) noexcept {
			if (nullptr != _handle)
				_handle.destroy();
		};

		inline void resume(void) const noexcept {
			_handle.resume();
		}
		inline bool done(void) noexcept {
			return _handle.done();
		}
		inline auto address(void) noexcept -> void* {
			return _handle.address();
		}
	private:
		std::coroutine_handle<promise> _handle;
	};
}