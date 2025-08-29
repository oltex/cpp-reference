#pragma once
#include "library/pattern/singleton.h"
#include "library/system/thread.h"
#include "library/system/wait_on_address.h"
#include "library/container/vector.h"

#include "library/container/intrusive/share_pointer.h"
#include "library/container/thread-local/pool.h"
#include "library/container/lockfree/queue.h"
#include "library/container/priority_queue.h"
#include "library/system/time.h"
#include <functional>

class thread_pool : public library::singleton<thread_pool, true> {
	friend class library::singleton<thread_pool, true>;
	using size_type = unsigned int;
	struct task {
		unsigned long _time;
		std::function<int(void)> _function;

		template <typename function, typename... argument>
		inline explicit task(function&& func, argument&&... arg) noexcept
			: _time(library::time_get_time()), _function(std::bind(std::forward<function>(func), std::forward<argument>(arg)...)) {
		};
		inline explicit task(task const&) noexcept = delete;
		inline explicit task(task&&) noexcept = delete;
		inline auto operator=(task const&) noexcept -> task & = delete;
		inline auto operator=(task&&) noexcept -> task & = delete;
		inline ~task(void) noexcept = default;

		inline bool execute(void) noexcept {
			for (;;) {
				switch (int time = _function()) {
				case 0:
					break;
				case -1:
					return false;
				default:
					_time += time;
					return true;
				}
			}
		}
	};
	template<bool multi_pop>
	class queue final : public library::lockfree::queue<task*, multi_pop> {
		using base = library::lockfree::queue<task*, multi_pop>;
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
		inline auto pop(void) noexcept -> task* requires(false == multi_pop) {
			auto result = base::pop();
			library::interlock_decrement(_size);
			return result;
		}
		inline auto pop(void) noexcept -> std::optional<task*> requires(true == multi_pop) {
			auto result = base::pop();
			if (result)
				library::interlock_decrement(_size);
			return result;
		}
		inline bool wait(unsigned long mili_second) noexcept {
			unsigned long compare = 0;
			library::wait_on_address(_size, compare, mili_second);
			return !(_size & 0x80000000);
		}
		inline void stop(void) noexcept {
			library::interlock_or(_size, 0x80000000);
			library::wake_by_address_single(_size);
		}
	};
	class worker final {
	};

	library::thread _scheduler_thread;
	queue<false> _scheduler_queue;
	library::vector<library::thread> _worker_thread;
	queue<true> _worker_queue;
public:
	inline explicit thread_pool(size_type worker) noexcept
		: _scheduler_thread(&thread_pool::schedule, 0, this) {
		_worker_thread.reserve(worker);
		for (size_type index = 0; index < worker; ++index)
			_worker_thread.emplace_back(&thread_pool::worker, 0, this);
	}
	inline explicit thread_pool(thread_pool const&) noexcept = delete;
	inline explicit thread_pool(thread_pool&&) noexcept = delete;
	inline auto operator=(thread_pool const&) noexcept -> thread_pool & = delete;
	inline auto operator=(thread_pool&&) noexcept -> thread_pool & = delete;
	inline ~thread_pool(void) noexcept {
		_worker_queue.stop();
		HANDLE handle[128];
		for (unsigned int index = 0; index < _worker_thread.size(); ++index)
			handle[index] = _worker_thread[index].data();
		library::handle::wait_for_multiple(_worker_thread.size(), handle, true, INFINITE);
		_worker_thread.clear();
	};

	template <typename function, typename... argument>
	inline void execute(function&& func, argument&&... arg) noexcept {
		_worker_queue.emplace(library::_thread_local::pool<task>::instance().allocate(std::forward<function>(func), std::forward<argument>(arg)...));
	}
private:
	inline void worker(void) noexcept {
		while (_worker_queue.wait(INFINITE)) {
			while (auto task = _worker_queue.pop()) {
				if (false == (*task)->execute())
					library::_thread_local::pool<thread_pool::task>::instance().deallocate(*task);
				else
					_scheduler_queue.emplace(*task);
			}
		}
	}
	inline void schedule(void) noexcept {
		library::priority_queue<task*> ready_queue;
		auto wait = INFINITE;
		while (_scheduler_queue.wait(wait)) {
			while (!_scheduler_queue.empty())
				ready_queue.emplace(_scheduler_queue.pop());
			auto time = library::time_get_time();
			wait = INFINITE;
			while (!ready_queue.empty()) {
				if (auto top = ready_queue.top(); time < top->_time) {
					wait = top->_time - time;
					break;
				}
				else {
					ready_queue.pop();
					_worker_queue.emplace(top);
				}
			}
		}
	}
};

// 워커 스레드에서 하나의 큐를 대상으로 WaitOnAddress에 성능 병목이 걸림
// 워크 스틸링 구조로 변경한 경우에도 문제가 생기는지 파악이 필요