#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "network.h"
#include "socket.h"
#include "domain.h"
#include "poll.h"

#include <iostream>

void func(void) noexcept {
	network::network network;

	//network::domain domain;
	//list<network::storage> storage = domain.get_storage(L"www.naver.com", nullptr);
	//for (auto& iter : storage)
	//	std::cout << iter.get_address() << std::endl;

	network::storage_ipv4 storage;
	storage.set_address("127.0.0.1");
	storage.set_port(1111);

	network::socket socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socket.set_linger(1, 0);
	socket.set_nonblocking(1);
	socket.bind(reinterpret_cast<network::storage&>(storage));
	socket.listen(SOMAXCONN);

	network::poll poll;
	poll.push(socket, POLLRDNORM);
	poll.execute(-1);

	//for (auto& iter : poll) {
	//	if (iter._revent & POLLRDNORM)
	//}

	system("pause");
}

void exit(void) noexcept {
	_CrtDumpMemoryLeaks();
}

int main(void) noexcept {
	atexit(exit);
	func();
	return 0;
}