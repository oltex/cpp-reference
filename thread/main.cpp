#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "thread.h"
#include "spin_lock.h"
#include "wait_on_address_lock.h"

#include <thread>
#include <iostream>
#include <conio.h>

struct alignas(64) my_str {
	int a;
	int b;
};

my_str mystr;

void function(void) noexcept {
	for(int i = 0 ; i < 200000000; ++i){
		mystr.a++;
	}
}


void function2(void) noexcept {
	for (int i = 0; i < 200000000; ++i) {
		mystr.b++;
	}
}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	thread::thread thread1(function, 0);
	thread::thread thread2(function2, 0);

	system("pause");
	thread1.resume();
	thread2.resume();

	thread1.join(INFINITE);
	thread2.join(INFINITE);
	std::cout << mystr.a << mystr.b << std::endl;
	system("pause");

	return 0;
}