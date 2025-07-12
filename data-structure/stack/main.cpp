#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "stack.h"
#include <stack>

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::stack<int> std_stack;

	library::stack<my_class> stack;
	stack.emplace(1);
	stack.pop();
	stack.emplace(2);
	stack.emplace(3);
	stack.emplace(4);
	stack.pop();
	stack.emplace(5);
	stack.pop();
	stack.emplace(6);
	stack.pop();
	stack.emplace(7);
	stack.emplace(8);

	stack.top().function();
	return 0;
}