#pragma once
#include "../coroutine.h"

namespace example {
	class Base {
	public:
		virtual ~Base() = default;
		virtual library::coroutine<library::promise> coroutineFunction() {
			co_return;
		};
		virtual void coroutineFunction2() {
			int a = 10;
			return;
		}
	};

	class Derived : public Base {
		//public:
			//library::system::coroutine<library::system::promise> coroutineFunction() override {
			//	int a = 10;
			//	co_return;
			//}
			//void coroutineFunction2() override {
			//	int a = 10;
			//	return;
			//}
	};

	//int main() {
	//	Base* obj = new Base();
	//	system("pause");
	//
	//	for (int j = 0; j < 10; j++) {
	//		auto rdtsc = __rdtsc();
	//		for (int i = 0; i < 1000000; ++i) {
	//			//obj->coroutineFunction2();
	//			auto task = obj->coroutineFunction();
	//			task.resume();
	//		}
	//		rdtsc = __rdtsc() - rdtsc;
	//		std::cout << rdtsc << std::endl;
	//	}
	//
	//	delete obj;
	//}

	library::coroutine<library::promise> test(void) noexcept {
		printf("1\n");
		//co_await library::suspend();
		printf("2\n");
		co_return;
	}

	inline void coroutine(void) noexcept {
		auto co = test();
		co.resume();
		// 디비작업이 완료됐구나!
		//co.resume();
		printf("3\n");
	}

}