#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace network {
	//class file_descriptor final {
	//public:
	//	inline void zero(void) noexcept {
	//		FD_ZERO(&_set);
	//	}
	//	inline void set(socket const& socket) noexcept {
	//		FD_SET(socket.data(), &_set);
	//	}
	//	inline auto is_set(socket const& socket) const noexcept -> int {
	//		return FD_ISSET(socket.data(), &_set);
	//	}
	//public:
	//	inline auto data(void) noexcept -> fd_set& {
	//		return _set;
	//	}
	//private:
	//	fd_set _set;
	//};

	//class select final {
	//public:
	//	inline auto execute(fd_set* read, fd_set* write, fd_set* exception, timeval* time) noexcept -> int {
	//		int result = ::select(0, read, write, exception, time);
	//		if (SOCKET_ERROR == result)
	//			DebugBreak();
	//		return result;
	//	}
	//};
}