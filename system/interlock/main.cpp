#include "interlock.h"

int main(void) noexcept {
	unsigned char a = 0;
	unsigned short b = 0;
	unsigned int c = 0;
	unsigned long long d = 0;
	void* e = nullptr;

	auto res1 = library::interlock_and(a, 1);
	auto res1 = library::interlock_and(b, 1);
	auto res1 = library::interlock_and(c, 1);
	auto res1 = library::interlock_and(d, 1);

	auto res1 = library::interlock_increment(b);
	auto res2 = library::interlock_increment(c);
	auto res3 = library::interlock_increment(d);

	auto res4 = library::interlock_decrement(b);
	auto res5 = library::interlock_decrement(c);
	auto res6 = library::interlock_decrement(d);

	auto res7 = library::interlock_exchange(a, 10);
	auto res8 = library::interlock_exchange(b, 10);
	auto res9 = library::interlock_exchange(c, 10);
	auto res10 = library::interlock_exchange(d, 10);
	auto res11 = library::interlock_exchange(e, (void*)10);

	auto res12 = library::interlock_exchange_add(c, 10);
	auto res13 = library::interlock_exchange_add(d, 10);

	auto res14 = library::interlock_compare_exhange(b, 20, 10);
	auto res15 = library::interlock_compare_exhange(c, 30, 20);
	auto res16 = library::interlock_compare_exhange(d, 30, 20);
	auto res17 = library::interlock_compare_exhange(e, (void*)20, (void*)10);

	//long test = 0;
	//_InlineInterlockedAdd(&test, 10);
	//InterlockedAdd(&test, 10);
	//InterlockedAdd64();
	//InterlockedExchangeAdd()

	return 0;
}