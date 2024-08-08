#pragma once
#pragma comment(lib,"ws2_32.lib")

#include "vector.h"
#include "socket.h"

#include <WinSock2.h>

namespace network {
	class poll final {
	public:
		using iterator = typename vector<pollfd>::iterator;
	public:
		//POLLRDNORM //POLLWRNORM
		inline void push(socket const& socket, short const event) noexcept {
			_pollfd.emplace_back(socket.data(), event);
		}
		inline void pop(socket const& socket) noexcept {
		}
		inline void execute(int const timeout) noexcept {
			WSAPoll(_pollfd.data(), _pollfd.size(), timeout);
		}
	public:
		inline auto begin(void) const noexcept -> iterator {
			return _pollfd.begin();
		}
		inline auto end(void) const noexcept -> iterator {
			return _pollfd.end();
		}
	public:
		inline void clear(void) noexcept {
			_pollfd.clear();
		}
	private:
		vector<pollfd> _pollfd;
	};
}