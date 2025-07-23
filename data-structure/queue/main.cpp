#include "../my_class.h"
#include <queue>
#include "queue.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	return 0;
}