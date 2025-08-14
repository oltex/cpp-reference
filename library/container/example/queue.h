#pragma once
#include "../queue.h"
#include "my_class.h"
#include <queue>

namespace example {
	inline void queue(void) noexcept {
		std::queue<int> std_queue;
		library::queue<my_class> queue;
		queue.emplace(1);
		queue.pop();
		queue.emplace(2);
		queue.emplace(3);
		queue.emplace(4);
		queue.pop();
		queue.emplace(5);
		queue.pop();
		queue.emplace(6);
		queue.pop();
		queue.emplace(7);
		queue.emplace(8);
		queue.clear();
		//queue.front().function();
		//queue.back().function();
	}
}