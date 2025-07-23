#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "priority_queue.h"
#include "../my_class.h"

#include <queue>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	return 0;
}