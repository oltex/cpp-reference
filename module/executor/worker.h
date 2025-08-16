#pragma once
#include "queue.h"
#include "system/thread.h"

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
		printf("worker_start\n");
		for (;;) {
			_queue.wait();
			for (;;) {
				auto task = deque();
				if (!task)
					break;
				task->resume();
			}
		}
	}
};