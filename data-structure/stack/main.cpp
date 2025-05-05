#include "../my_class.h"
#include "stack.h"
#include <stack>

int main(void) noexcept {
	std::stack<int> std_stack;
	library::data_structure::stack<my_class> stack;
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