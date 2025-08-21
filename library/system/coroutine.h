#pragma once
#include "../memory.h"
#include <coroutine>
#include <iostream>
#include <Windows.h>

namespace library {
	class awaiter {
	public:
		inline explicit awaiter(void) noexcept = default;
		inline explicit awaiter(awaiter const&) noexcept = delete;
		inline explicit awaiter(awaiter&&) noexcept = delete;
		inline auto operator=(awaiter const&) noexcept -> awaiter & = delete;
		inline auto operator=(awaiter&&) noexcept -> awaiter & = delete;
		inline ~awaiter(void) noexcept = default;

		inline bool await_ready(void) noexcept {
			return true;
		}
		inline void await_suspend(std::coroutine_handle<void> handle) noexcept {
		}
		inline int await_resume(void) noexcept {
			return 0;
		}
	};

	template<typename type>
	class promise {
	public:
		inline explicit promise(void) noexcept = default;
		inline explicit promise(promise const&) noexcept = delete;
		inline explicit promise(promise&&) noexcept = delete;
		inline auto operator=(promise const&) noexcept -> promise & = delete;
		inline auto operator=(promise&&) noexcept -> promise & = delete;
		inline ~promise(void) noexcept = default;

		inline auto get_return_object(void) noexcept -> std::coroutine_handle<type> {
			return std::coroutine_handle<type>::from_promise(static_cast<type&>(*this));
		}
		inline auto initial_suspend(void) noexcept -> std::suspend_never {
			return std::suspend_never();
		}
		inline auto final_suspend(void) noexcept -> std::suspend_never {
			return std::suspend_never();
		}
		//inline auto yield_value(int result) noexcept -> std::suspend_always {
		//	return std::suspend_always();
		//}
		//inline auto await_transform(int result) noexcept -> suspend {
		//	printf("await transform\n");
		//	return suspend();
		//}
		inline void return_void(void) noexcept {
		}
		//inline void return_value(int result) noexcept {
		//}
		inline void unhandled_exception(void) noexcept {
			__debugbreak();
		}

		inline static void* operator new(size_t size) noexcept {
			return library::allocate(size);
		}
		inline static void operator delete(void* pointer, size_t size) noexcept {
			library::deallocate(pointer);
		}
	};

	template<typename type>
	class coroutine {
	protected:
		std::coroutine_handle<type> _handle;
	public:
		using promise_type = type;
		inline coroutine(std::coroutine_handle<type> handle) noexcept
			: _handle(handle) {
		}
		inline explicit coroutine(coroutine const&) noexcept = delete;
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
			//if (nullptr != _handle)
				//_handle.destroy();
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
		inline auto promise(void) noexcept -> type& {
			return _handle.promise();
		}
		inline auto data(void) noexcept -> std::coroutine_handle<type>& {
			return _handle;
		}
	};
}