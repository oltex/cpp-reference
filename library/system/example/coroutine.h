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

	library::coroutine<library::promise> test(void) {
		//1
		co_await; //string //��� ��û
		//3
		co_return;
	}

	inline void coroutine(void) noexcept {
		// gqcs

		auto co = test();
		co.resume();
		//2 �����ȸ�� ��û�߱���!
		// �����ȸ�� ���� redis [](){������ pqcs *co ����};
		continue;


		// ����۾��� �Ϸ�Ʊ���!
		co.resume();
	}

}