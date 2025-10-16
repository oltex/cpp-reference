#pragma once
#include "socket.h"
#include <WS2tcpip.h>

namespace library {
	class file_descript final {
	private:
		using size_type = unsigned int;
	public:
		inline explicit file_descript(void) noexcept = default;
		inline explicit file_descript(file_descript const&) noexcept = delete;
		inline explicit file_descript(file_descript&&) noexcept = delete;
		inline auto operator=(file_descript const&) noexcept -> file_descript & = delete;
		inline auto operator=(file_descript&&) noexcept -> file_descript & = delete;
		inline ~file_descript(void) noexcept = default;

		inline void zero(void) noexcept {
			FD_ZERO(&_fd_set);
		}
		inline void set(socket& socket) noexcept {
			FD_SET(socket.data(), &_fd_set);
		}
		inline auto is_fd_set(socket& socket) const noexcept -> int {
			return FD_ISSET(socket.data(), &_fd_set);
		}
		inline auto data(void) noexcept -> fd_set& {
			return _fd_set;
		}
		inline auto size(void) const noexcept -> size_type {
			return _fd_set.fd_count;
		}
	private:
		fd_set _fd_set;
	};
}