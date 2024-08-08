#pragma once
#pragma comment(lib,"ws2_32.lib")

#include "vector.h"
#include "socket.h"

#include <WinSock2.h>

namespace network {
	class poll final {
	public:
		struct node final {
			socket _socket;
			short _revent;
		};
		using iterator = typename vector<node>::iterator;
	public:
		//POLLRDNORM //POLLWRNORM
		inline void push(socket const& socket, short const event) noexcept {
			_pollfd.emplace_back(socket.data(), event);
		}
		inline void execute(int const timeout) noexcept {
			WSAPoll(_pollfd.data(), _pollfd.size(), timeout);
			for (auto& iter : _pollfd)
				_result.emplace_back(socket(iter.fd), iter.revents);
		}
	public:
		inline auto begin(void) const noexcept -> iterator {
			return _result.begin();
		}
		inline auto end(void) const noexcept -> iterator {
			return _result.end();
		}
	public:
		inline void clear(void) noexcept {
			_pollfd.clear();
			_result.clear();
		}
	private:
		vector<pollfd> _pollfd;
		vector<node> _result;
	};
}