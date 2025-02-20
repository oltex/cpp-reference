#include "coroutine.h"

class Base {
public:
	virtual ~Base() = default;
	virtual system_component::coroutine<system_component::promise> coroutineFunction() {
		co_return;
	};
	virtual void coroutineFunction2() {
		int a = 10;
		return;
	}
};

class Derived : public Base {
//public:
	//system_component::coroutine<system_component::promise> coroutineFunction() override {
	//	int a = 10;
	//	co_return;
	//}
	//void coroutineFunction2() override {
	//	int a = 10;
	//	return;
	//}
};

int main() {
	Base* obj = new Base();
	system("pause");

	for (int j = 0; j < 10; j++) {
		auto rdtsc = __rdtsc();
		for (int i = 0; i < 1000000; ++i) {
			//obj->coroutineFunction2();
			auto task = obj->coroutineFunction();
			task.resume();
		}
		rdtsc = __rdtsc() - rdtsc;
		std::cout << rdtsc << std::endl;
	}

	delete obj;
}
