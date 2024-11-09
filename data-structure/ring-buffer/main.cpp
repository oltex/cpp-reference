#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "ring_buffer_v2.h"
#include <iostream>
#include <Windows.h>

void func(void) noexcept {
	srand(6530985434);
	data_structure::ring_buffer ring;
	unsigned char str[] = "!1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwz 123456!";
	unsigned int index = 0;

	for (;;) {
		// push
		unsigned int push = rand() % 121;
		if (ring.remain() < push)
			ring.reserve(ring.size() + push);
		if (ring.remain() >= push) {
			size_t size = (120 - index);
			size = size < push ? size : push;

			ring.push(str + index, size);
			index = (index + size) % 120;
			ring.push(str + index, push - size);
			index = (index + push - size) % 120;
		}

		//pop
		unsigned  int pop = rand() % 121;
		if (ring.size() >= pop) {
			unsigned char buf[512];
			ring.peek(buf, pop);
			buf[pop] = '\0';
			std::cout << buf;

			ring.pop(pop);
		}
	}
}

//void func(void) noexcept {
//	srand(653098543432);
//	data_structure::ring_buffer ring;
//	unsigned char str[] = "!1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwz 123456!";
//	int index = 0;
//
//	for (;;) {
//		// push
//		int push = rand() % 121;
//		//if (ring.remain() < size)
//		//	ring.reserve(ring.size() + size);
//		if (ring.remain() >= push) {
//			size_t size = (120 - index);
//			size = size < push ? size : push;
//
//			{
//				size_t once = ring.at_once_receive();
//				once = size < once ? size : once;
//
//				memcpy(ring.data() + ring.get_rear(), str + index, once);
//				if (size != once)
//					memcpy(ring.data(), str + index + once, size - once);
//				ring.move_rear(size);
//				index = (index + size) % 120;
//			}
//
//			ring.push(str + index, push - size);
//			index = (index + push - size) % 120;
//		}
//
//		//pop
//		int pop = rand() % 121;
//		if (ring.size() >= pop) {
//			unsigned char buf[512];
//			ring.peek(buf, pop);
//			buf[pop] = '\0';
//			std::cout << buf;
//
//			ring.pop(pop);
//		}
//	}
//}

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	func();
	return 0;
}