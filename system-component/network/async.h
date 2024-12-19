#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "../window/window.h"
#include "socket.h"
#include <WinSock2.h>

namespace network {
	class async final {
	public:
		inline void select(socket& socket, window::window& window, unsigned int const message, long const event_) const noexcept {
			if (SOCKET_ERROR == WSAAsyncSelect(socket.data(), window.data(), message, event_))
				__debugbreak();
		}
	};
}