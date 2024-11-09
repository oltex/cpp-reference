#define _CRTDBGmap_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "object_pool.h"
#include <vector>
#include <intrin.h>
#include <iostream>
#include "../my_class.h"

int nansu[10000000];
int* save[10000000];
int index = 0;

void func(void) noexcept {
	data_structure::object_pool<int> object_pool;
	for (size_t i = 0; i < 10000000; i++)
		object_pool.release(object_pool.allocate());
	index = 0;
	__int64 tsc = __rdtsc();
	for (int i = 0; i < 10000000; ++i) {
		if (0 == nansu[i]) {
			if (object_pool.empty())
				save[index++] = &object_pool.allocate();
			else
				save[index++] = &object_pool.get();
		}
		else {
			if (0 != index)
				object_pool.release(*save[--index]);
		}
	}
	tsc = __rdtsc() - tsc;
	std::cout << "object_pool :" << tsc << std::endl;

}
void func2(void) noexcept {
	index = 0;
	__int64 tsc = __rdtsc();
	for (int i = 0; i < 10000000; ++i) {
		if (0 == nansu[i]) {
			save[index++] = (int*)malloc(sizeof(int));
		}
		else {
			if (0 != index)
				free(save[--index]);
		}
	}
	tsc = __rdtsc() - tsc;
	std::cout << "malloc      :" << tsc << std::endl;
}

//void func(data_structure::object_pool<int>& object_pool) {
//	__int64 tsc = __rdtsc();
//	for (int i = 0; i < 100000000; ++i) {
//		int& a = object_pool.get();
//		object_pool.release(a);
//	}
//	tsc = __rdtsc() - tsc;
//	std::cout << "object_pool :" << tsc << std::endl;
//}
//
//void func2(void) {
//	__int64 tsc = __rdtsc();
//	for (int i = 0; i < 100000000; ++i) {
//		auto a = (int*)malloc(sizeof(int));
//		free(a);
//	}
//	tsc = __rdtsc() - tsc;
//	std::cout << "malloc      :" << tsc << std::endl;
//}



int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	system("pause");
	data_structure::object_pool<my_class> object_pool;
	object_pool.release(object_pool.allocate());

	//for (int i = 0; i < 10000000; ++i)
	//	nansu[i] = rand() % 2;

	//for (int i = 0; i < 10; ++i) {
	//	func();
	//	func2();
	//}
	//for (int i = 0; i < 10; ++i) {
	//	func(object_pool);
	//	func2();
	//}

	return 0;
}