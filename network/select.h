#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace network {
	class select final {
	public:
		inline auto execute(fd_set* read, fd_set* write, fd_set* exception, timeval* time) noexcept -> int {
			int result = ::select(0, read, write, exception, time);
			if (SOCKET_ERROR == result)
				DebugBreak();
			return result;
		}
	};
}