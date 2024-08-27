#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket.h"
#include <WinSock2.h>

namespace network {
	class select final {
	public:
		enum type : char {
			read = 0x1, write = 0x2, exception = 0x4
		};
	public:
		inline void zero(char const type) noexcept {
			if (type::read & type)
				FD_ZERO(&_read_set);
			if (type::write & type)
				FD_ZERO(&_write_set);
			if (type::exception & type)
				FD_ZERO(&_exception_set);
		}
		inline void set(socket const& socket, char const type) noexcept {
			if (type::read & type)
				FD_SET(socket.data(), &_read_set);
			if (type::write & type)
				FD_SET(socket.data(), &_write_set);
			if (type::exception & type)
				FD_SET(socket.data(), &_exception_set);
		}
		inline auto execute(char const type, timeval const* const time) noexcept -> int {
			auto read_set = type::read & type ? 0 != _read_set.fd_count ? &_read_set : nullptr : nullptr;
			auto write_set = type::write & type ? 0 != _write_set.fd_count ? &_write_set : nullptr : nullptr;
			auto exception_set = type::exception & type ? 0 != _exception_set.fd_count ? &_exception_set : nullptr : nullptr;
			if (nullptr == read_set && nullptr == write_set && nullptr == exception_set)
				return 0;

			int result = ::select(0, read_set, write_set, exception_set, time);
			if (SOCKET_ERROR == result)
				DebugBreak();
			return result;
		}
		inline auto is_set(socket const& socket, type const type) const noexcept -> int {
			switch (type) {
			case read:
				return FD_ISSET(socket.data(), &_read_set);
			case write:
				return FD_ISSET(socket.data(), &_write_set);
			case exception:
				return FD_ISSET(socket.data(), &_exception_set);
			}
			return 0;
		}
	private:
		fd_set _read_set, _write_set, _exception_set;
	};
}