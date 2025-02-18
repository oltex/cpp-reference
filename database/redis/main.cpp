#include "redis.h"

int main(void) noexcept {
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(version, &data);

	database::redis client;
	client.connect();

	client.set("hello", "42");
	client.commit();

	//client.get("hello", 10);
	client.get([](cpp_redis::reply& reply) {
		std::cout << reply << std::endl;
		}, "hello", 10);
	client.sync_commit();

	//auto set_future = client.get("hello");
	//! also support std::future
	//! std::future<cpp_redis::reply> get_reply = client.get("hello");

	//client.commit();
	//auto replay = set_future.get();
	//! or client.commit(); for asynchronous call}
	//!

}