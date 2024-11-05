#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <intrin.h>

namespace network {
	inline static void start_up(void) noexcept {
		WSAData wsadata;
		if (0 != WSAStartup(0x0202, &wsadata))
			__debugbreak();
	};
	inline static void clean_up(void) noexcept {
		if (SOCKET_ERROR == WSACleanup())
			__debugbreak();
	};
	inline static auto wsa_wait_for_multiple_object(unsigned long const count, HANDLE* handle, bool const wait_all, unsigned long const timeout, bool const alertable) noexcept -> unsigned long {
		return WSAWaitForMultipleEvents(count, handle, wait_all, timeout, alertable);
	}
}