#include "redis.h"

int main(void) noexcept {
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(version, &data);

	library::database::redis client;
	client.connect("127.0.0.1", 6379, nullptr, 0, 0, 0);

	//client.set("hello", "42");
	client.get("1", [](cpp_redis::reply& reply) {
		std::cout << reply << std::endl;
		});
	client.commit();

	Sleep(10000);
	//client.sync_commit();k

	//auto set_future = client.get("hello");
	//! also support std::future
	//! std::future<cpp_redis::reply> get_reply = client.get("hello");

	//client.commit();
	//auto replay = set_future.get();
	//! or client.commit(); for asynchronous call}
	//!

}