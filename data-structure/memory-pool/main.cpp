#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "memory_pool.h"

#pragma pack(push, 1)
struct MyStruct {
	inline explicit MyStruct() {
		p = 30;
		a = 1234;
	}
	inline ~MyStruct() {
		int a = 10;
	}
	char p;
	int a;
	int b;
	int c;
	int d;
};
#pragma pack(pop)

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int a = alignof(MyStruct);
	library::data_structure::memory_pool<MyStruct, true, true> memory_pool;

	auto& b = memory_pool.allocate();
	memory_pool.deallocate(b);
	return 0;
}