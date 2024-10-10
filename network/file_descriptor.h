#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace network {
	class file_descriptor final {
	private:
		using size_type = unsigned int;
	public:
		inline void zero(void) noexcept {
			FD_ZERO(&_set);
		}
		inline void set(socket const& socket) noexcept {
			FD_SET(socket.data(), &_set);
		}
		inline auto is_set(socket const& socket) const noexcept -> int {
			return FD_ISSET(socket.data(), &_set);
		}
	public:
		inline auto data(void) noexcept -> fd_set& {
			return _set;
		}
		inline auto size(void) const noexcept -> size_type {
			return _set.fd_count;
		}
	private:
		fd_set _set;
	};
}