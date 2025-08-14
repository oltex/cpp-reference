#pragma once
#include "../priority_queue.h"
#include "my_class.h"

#include <queue>

namespace example {
	inline void priority_queue(void) noexcept {
		std::priority_queue<int> std_queue;
		library::priority_queue<int> queue;
		queue.emplace(4);
		queue.emplace(3);
		queue.emplace(2);
		queue.emplace(1);
		queue.emplace(0);
		queue.emplace(5);
		queue.emplace(6);
		queue.emplace(7);
		queue.emplace(8);
		queue.emplace(9);
		library::priority_queue<int> queue2;
		queue2.swap(queue);
		while (!queue.empty()) {
			std::cout << queue.top() << std::endl;
			queue.pop();
		}
		while (!queue2.empty()) {
			std::cout << queue2.top() << std::endl;
			queue2.pop();
		}
	}
}