#pragma once
#include "pattern/singleton.h"
#include "system/thread.h"
#include "system/interlock.h"
#include "system/wait_on_address.h"
#include "container/lockfree/queue.h"
#include "container/vector.h"

#include <coroutine>

namespace moduler {
	template<typename _promise_type>
	class coroutine final {
	public:
		using promise_type = _promise_type;
		inline explicit coroutine(std::coroutine_handle<promise_type> handle) noexcept
			: _handle(handle) {
		}
		inline explicit coroutine(coroutine const&) noexcept = default;
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
		inline auto promise(void) noexcept -> promise_type& {
			return _handle.promise();
		}
	private:
		std::coroutine_handle<promise_type> _handle;
	};
	class promise final {
	public:
		inline explicit promise(void) noexcept = default;
		inline explicit promise(promise const&) noexcept = delete;
		inline explicit promise(promise&&) noexcept = delete;
		inline auto operator=(promise const&) noexcept -> promise & = delete;
		inline auto operator=(promise&&) noexcept -> promise & = delete;
		inline ~promise(void) noexcept = default;

		inline auto get_return_object(void) noexcept -> coroutine<promise> {
			return coroutine<promise>(std::coroutine_handle<promise>::from_promise(*this));
		}
		inline auto initial_suspend(void) noexcept -> std::suspend_always {
			//printf("initial suspend\n");
			return std::suspend_always();
		}
		inline auto final_suspend(void) noexcept -> std::suspend_never {
			//printf("final suspend\n");
			return std::suspend_never();
		}
		inline auto yield_value(int result) noexcept -> std::suspend_always {
			printf("yield value\n");
			return std::suspend_always();
		}
		//inline auto await_transform(int result) noexcept -> suspend {
		//	printf("await transform\n");
		//	return suspend();
		//}
		inline void return_void(void) noexcept {
			//printf("return void\n");
		}
		//inline void return_value(int result) noexcept {
		//	printf("return value\n");
		//}
		inline void unhandled_exception(void) noexcept {
			printf("unhandled exception\n");
		}

		inline static void* operator new(size_t size) noexcept {
			return library::allocate(size);
		}
		inline static void operator delete(void* pointer, size_t size) noexcept {
			library::deallocate(pointer);
		}
	};

	class queue final : public library::lockfree::queue<coroutine<promise>> {
		using base = library::lockfree::queue<coroutine<promise>>;
		unsigned long _size;
	public:
		inline explicit queue(void) noexcept
			: _size(0), base() {
		};
		inline explicit queue(queue const&) noexcept = default;
		inline explicit queue(queue&&) noexcept = default;
		inline auto operator=(queue const&) noexcept -> queue&;
		inline auto operator=(queue&&) noexcept -> queue&;
		inline ~queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			library::interlock_increment(_size);
			base::emplace(std::forward<argument>(arg)...);
			library::wake_by_address_single(_size);
		}
		inline auto pop(void) noexcept -> std::optional<coroutine<promise>> {
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

	class worker final {
		queue _queue;
		library::thread _thread;
	public:
		inline explicit worker(void) noexcept
			: _thread(&worker::function, 0, this) {
		}
		inline explicit worker(worker const&) noexcept = default;
		inline explicit worker(worker&&) noexcept = default;
		inline auto operator=(worker const&) noexcept -> worker&;
		inline auto operator=(worker&&) noexcept -> worker&;
		inline ~worker(void) noexcept = default;

		inline void enque(coroutine<promise>& coroutine) noexcept {
			_queue.emplace(coroutine);
		}
		inline void function(void) noexcept {
			printf("worker_start\n");
			for (;;) {
				_queue.wait();
				for (;;) {
					auto result = _queue.pop();
					if (!result)
						break;
					result->resume();
				}
			}
		}
	};

	class executor final {
		using size_type = unsigned int;
		size_type _round_robin;
		library::vector<worker> _vector;
	public:
		inline explicit executor(size_type worker) noexcept
			: _round_robin(0) {
			_vector.resize(worker);
		}
		inline explicit executor(executor const&) noexcept = default;
		inline explicit executor(executor&&) noexcept = default;
		inline auto operator=(executor const&) noexcept -> executor&;
		inline auto operator=(executor&&) noexcept -> executor&;
		inline ~executor(void) noexcept = default;

		template <typename function, typename... argument>
		inline void registed(function&& func, argument&&... arg) noexcept {
			coroutine<promise> t = std::invoke(std::forward<function>(func), std::forward<argument>(arg)...);
			_vector[_round_robin].enque(t);
		}
	};
}