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
				FD_ZERO(&_read_set);
			if (type::exception & type)
				FD_ZERO(&_read_set);
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
			int result = ::select(0,
				type::read & type ? &_read_set : nullptr,
				type::write & type ? &_write_set : nullptr,
				type::exception & type ? &_exception_set : nullptr,
				time);
			if (SOCKET_ERROR == result)
				DebugBreak();
			return result;
		}
		inline auto is(socket const& socket, type const type) const noexcept -> int {
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