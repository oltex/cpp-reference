#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace network {
	class select final {
	public:
		inline explicit select(void) noexcept = default;
		inline explicit select(select const& rhs) noexcept = delete;
		inline explicit select(select&& rhs) noexcept = delete;
		inline auto operator=(select const& rhs) noexcept -> select & = delete;
		inline auto operator=(select&& rhs) noexcept -> select & = delete;
		inline ~select(void) noexcept = default;
	public:
		inline auto execute(fd_set* read, fd_set* write, fd_set* exception, timeval* time) noexcept -> int {
			int result = ::select(0, read, write, exception, time);
			if (SOCKET_ERROR == result)
				__debugbreak();
			return result;
		}
	};
}