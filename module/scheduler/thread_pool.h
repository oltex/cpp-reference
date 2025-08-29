#pragma once
#include "library/pattern/singleton.h"
#include "library/system/thread.h"
#include "library/system/wait_on_address.h"
#include "library/container/vector.h"

#include "library/container/intrusive/share_pointer.h"
#include "library/container/thread-local/pool.h"
#include "library/container/lockfree/queue.h"
#include <functional>


//		class queue final : public library::lockfree::queue<task, false> {
//		public:
//			inline explicit queue(void) noexcept = default;
//			inline explicit queue(queue const&) noexcept = delete;
//			inline explicit queue(queue&&) noexcept = delete;
//			inline auto operator=(queue const&) noexcept -> queue & = delete;
//			inline auto operator=(queue&&) noexcept -> queue & = delete;
//			inline ~queue(void) noexcept = default;
//
//			//inline void wake(void) noexcept {
//			//	library::wake_by_address_single();
//			//}
//			//inline bool wait(void* compare, unsigned long const wait_time) noexcept {
//			//	library::wait_on_address()
//			//	return system_component::wait_on_address::wait(&_size, compare, sizeof(size_type), wait_time);
//			//}
//		};

class thread_pool : public library::singleton<thread_pool, true> {
	friend class library::singleton<thread_pool, true>;
	using size_type = unsigned int;

	struct node : public library::intrusive::share_pointer_hook<0> {
		unsigned long _time;
		std::function<int(void)> _function;

		template <typename function, typename... argument>
		inline explicit node(function&& func, argument&&... arg) noexcept
			:_function(std::bind(std::forward<function>(func), std::forward<argument>(arg)...)) {
		};
		inline explicit node(node const&) noexcept = delete;
		inline explicit node(node&&) noexcept = delete;
		inline auto operator=(node const&) noexcept -> node & = delete;
		inline auto operator=(node&&) noexcept -> node & = delete;
		inline ~node(void) noexcept = default;

		template<size_t index>
		inline void destructor(void) noexcept;
		template<>
		inline void destructor<0>(void) noexcept {
			library::_thread_local::pool<node>::instance().deallocate(this);
		}
	};
	using task = library::intrusive::share_pointer<node, 0>;
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

		inline void enque(task task) noexcept {
			_queue.emplace(task);
		}
		inline auto deque(void) noexcept {
			return _queue.pop();
		}
		inline void function(void) noexcept {
			for (;;) {
				_queue.wait();
				for (;;) {
					auto task = deque();
					if (!task)
						break;
				}
			}
		}
	};

	library::vector<worker> _worker;
public:
	inline explicit thread_pool(size_type worker) noexcept {
		for (size_type index = 0; index < worker; ++index)
			_worker.emplace_back();
	}
	inline explicit thread_pool(thread_pool const&) noexcept = delete;
	inline explicit thread_pool(thread_pool&&) noexcept = delete;
	inline auto operator=(thread_pool const&) noexcept -> thread_pool & = delete;
	inline auto operator=(thread_pool&&) noexcept -> thread_pool & = delete;
	inline ~thread_pool(void) noexcept;

	template <typename function, typename... argument>
	inline void execute(function&& func, argument&&... arg) noexcept {
		library::_thread_local::pool<node>::instance().allocate(std::forward<function>(func), std::forward<argument>(arg)...);
		//_worker[_round_robin].enque(coroutine.data());
	}
};