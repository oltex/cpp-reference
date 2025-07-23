#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "circular_queue.h"
#include "../my_class.h"

#include <queue>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//std::queue<int> std_queue;
	//library::queue<my_class> queue;
	//queue.emplace(1);
	//queue.pop();
	//queue.emplace(2);
	//queue.emplace(3);
	//queue.emplace(4);
	//queue.pop();
	//queue.emplace(5);
	//queue.pop();
	//queue.emplace(6);
	//queue.pop();
	//queue.emplace(7);
	//queue.emplace(8);
	//queue.clear();
	////queue.front().function();
	////queue.back().function();


	//library::circular_queue<int> queue;
	//queue.emplace(0);
	//queue.emplace(1);
	//queue.pop();
	//queue.emplace(2);
	//queue.emplace(3);
	//queue.pop();
	//queue.emplace(4);
	//queue.emplace(5);
	//queue.emplace(6);
	//queue.pop();
	//queue.emplace(7);
	//queue.emplace(8);
	//queue.emplace(9);
	//auto size = queue.size();
	//auto remain = queue.remain();
	//auto capacity = queue.capacity();
	//auto empty = queue.empty();
	//auto full = queue.full();
	//queue.pop();
	//queue.emplace(10);
	//queue.emplace(11);
	//queue.pop();
	//queue.pop();
	//srand(0);
	//static int a = 0;
	//for (int i = 0; i < 10000; ++i) {
	//	switch (rand() % 3) {
	//	case 0:
	//	case 2:
	//		queue.emplace(a++);
	//		break;
	//	case 1:
	//		queue.pop();
	//		break;
	//		//case 2:
	//		//	queue.clear();
	//		//	break;
	//	}
	//}
	//std::cout << "for" << std::endl;
	//for (auto iter = queue.begin(); iter != queue.end(); iter++) {
	//	std::cout << (*iter) << std::endl;
	//}
	//std::cout << "foreach" << std::endl;
	//for (auto& iter : queue) {
	//	std::cout << iter << std::endl;
	//}
	//library::circular_queue<int> queue2(std::move(queue));
	//std::cout << "while" << std::endl;
	//while (!queue2.empty()) {
	//	std::cout << queue2.top() << std::endl;
	//	queue2.pop();
	//}


	//std::priority_queue<int> std_queue;
	//library::priority_queue<int> queue;
	//queue.emplace(4);
	//queue.emplace(3);
	//queue.emplace(2);
	//queue.emplace(1);
	//queue.emplace(0);
	//queue.emplace(5);
	//queue.emplace(6);
	//queue.emplace(7);
	//queue.emplace(8);
	//queue.emplace(9);
	//library::priority_queue<int> queue2;
	//queue2.swap(queue);
	//while (!queue.empty()) {
	//	std::cout << queue.top() << std::endl;
	//	queue.pop();
	//}
	//while (!queue2.empty()) {
	//	std::cout << queue2.top() << std::endl;
	//	queue2.pop();
	//}

	return 0;
}