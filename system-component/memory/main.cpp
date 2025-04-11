#include "memory.h"
#include "iostream"
struct my_struct {
	int a;
};


int main(void) noexcept {

	for (;;) {
		//int* a = reinterpret_cast<int*>(::malloc(sizeof(int)));
		int* a = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
		printf("%p\n", a);
	}
	//int* a1 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
	//int* a2 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
	//int* a3 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
	//int* a4 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
	//int* a5 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));
	//int* a6 = reinterpret_cast<int*>(::_aligned_malloc(sizeof(int), 4));

	//my_struct* a = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a1 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a2 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a3 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a4 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a5 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));
	//my_struct* a6 = reinterpret_cast<my_struct*>(::malloc(sizeof(my_struct)));

	//auto a = library::system_component::memory::allocate(10);
	//library::system_component::memory::deallocate(a);
	return 0;
}