#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "circular_queue.h"
#include <iostream>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::data_structure::circular_queue<int> queue;

	//queue.emplace(0);
	//queue.emplace(1);
	//queue.emplace(2);
	//queue.emplace(3);
	//queue.emplace(4);
	//queue.emplace(5);
	//queue.emplace(6);
	//queue.emplace(7);
	//queue.emplace(8);
	//queue.emplace(9);
	//queue.pop();
	//queue.emplace(10);
	//queue.emplace(11);
	//queue.pop();
	//queue.pop();
	//queue.pop();
	//queue.pop();
	//queue.pop();

	srand(0);
	static int a = 0;
	for (int i = 0; i < 10000; ++i) {
		switch (rand() % 3) {
		case 0:
		case 2:
			queue.emplace(a++);
			break;
		case 1:
			queue.pop();
			break;
		//case 2:
		//	queue.clear();
		//	break;
		}
	}

	std::cout << "for" << std::endl;
	for (auto iter = queue.begin(); iter != queue.end(); iter++) {
		std::cout << (*iter) << std::endl;
	}

	std::cout << "foreach" << std::endl;
	for (auto& iter : queue) {
		std::cout << iter << std::endl;
	}

	std::cout << "while" << std::endl;
	while (!queue.empty()) {
		std::cout << queue.top() << std::endl;
		queue.pop();
	}

	return 0;
}