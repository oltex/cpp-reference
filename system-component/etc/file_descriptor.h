#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace library::system_component::network {
	class file_descriptor final {
	private:
		using size_type = unsigned int;
	public:
		inline explicit file_descriptor(void) noexcept = default;
		inline explicit file_descriptor(file_descriptor const& rhs) noexcept = delete;
		inline explicit file_descriptor(file_descriptor&& rhs) noexcept = delete;
		inline auto operator=(file_descriptor const& rhs) noexcept -> file_descriptor & = delete;
		inline auto operator=(file_descriptor&& rhs) noexcept -> file_descriptor & = delete;
		inline ~file_descriptor(void) noexcept = default;
	public:
		inline void zero(void) noexcept {
			FD_ZERO(&_set);
		}
		inline void set(socket& socket) noexcept {
			FD_SET(socket.data(), &_set);
		}
		inline auto is_set(socket& socket) const noexcept -> int {
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