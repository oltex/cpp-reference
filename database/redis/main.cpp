#include <iostream>
#include <cpp_redis/cpp_redis>
#pragma comment (lib, "cpp_redis.lib")
#pragma comment (lib, "tacopie.lib")
#pragma comment (lib, "ws2_32.lib")

int main()
{
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(version, &data);

	cpp_redis::client client;
	client.connect();

	client.set("hello", "42");
	client.commit();

	//client.get("hello", [](cpp_redis::reply& reply) {
	//	std::cout << reply << std::endl;
	//	});
	auto set_future = client.get("hello");
	//! also support std::future
	//! std::future<cpp_redis::reply> get_reply = client.get("hello");

	client.commit();
	auto replay = set_future.get();
	//! or client.commit(); for asynchronous call}
	Sleep(1000);
}