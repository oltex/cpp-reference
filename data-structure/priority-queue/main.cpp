#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "priority_queue.h"

#include <iostream>
#include <queue>

class A {
public:
	int _a;
};

inline static auto test(int a, int b) noexcept {
	return a < b;
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	library::data_structure::priority_queue<int, algorithm::predicate::less<int>> queue;
	library::data_structure::priority_queue<int, test> queue2;

	queue.emplace(4);
	queue2.emplace(4);
	//queue.emplace(3);
	//queue.emplace(2);
	//queue.emplace(1);
	//queue.emplace(0);
	//queue.emplace(5);
	//queue.emplace(6);
	//queue.emplace(7);
	//queue.emplace(8);
	//queue.emplace(9);

	//while (!queue.empty()) {
	//	std::cout << queue.top() << std::endl;
	//	queue.pop();
	//}

	return 0;
}