#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <intrin.h>

namespace network::window_socket_api {
	inline static auto wait_for_multiple_event(unsigned long const count, HANDLE* handle, bool const wait_all, unsigned long const timeout, bool const alertable) noexcept -> unsigned long {
		return WSAWaitForMultipleEvents(count, handle, wait_all, timeout, alertable);
	}
}