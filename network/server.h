#pragma once
#include "session.h"

#include "network.h"
#include "select.h"
#include "socket.h"
#include "list.h"

//template<typename session_type>
class server final {
	//static_assert(std::is_base_of<session, session_type>::value);
public:
	inline explicit server(void) noexcept
		: _listen(AF_INET, SOCK_STREAM, IPPROTO_TCP) {
		network::storage_ipv4 storage;
		storage.set_address(INADDR_ANY);
		storage.set_port(5000);

		_listen.set_linger(1, 0);
		_listen.set_nonblocking(1);
		_listen.set_tcp_nodelay(1);

		_listen.bind(reinterpret_cast<network::storage&>(storage));
		_listen.listen(SOMAXCONN);
	}
	inline ~server(void) noexcept {
	}
public:
	inline void receive(void) noexcept {
		_select.zero(network::select::read);
		_select.set(_listen, network::select::read);
		for (auto& iter : _session)
			_select.set(iter._socket, network::select::read);

		timeval time{};
		if (0 >= _select.execute(network::select::read, &time))
			return;

		//accept
		if (_select.is_set(_listen, network::select::read)) {
			auto [sock, stor] = _listen.accept();
			auto& iter = _session.emplace_back(std::move(sock), stor);
			iter.accept();
		}

		//receive
		for (auto& iter : _session) {
			if (!_select.is_set(iter._socket, network::select::read))
				continue;
			int result = iter._socket.receive(
				(char*)(iter._recv_ring_buffer.data() + iter._recv_ring_buffer.get_rear())
				, (int)iter._recv_ring_buffer.at_once_receive(), 0);
			if (SOCKET_ERROR != result && 0 != result) {
				iter._recv_ring_buffer.move_rear(result);
				iter.receive();
			}
		}
	};
	inline void send(void) noexcept {
		_select.zero(network::select::write);
		for (auto& iter : _session)
			if (INVALID_SOCKET != iter._socket.data() && 0 < iter._send_ring_buffer.size())
				_select.set(iter._socket, network::select::write);

		timeval time{};
		if (0 >= _select.execute(network::select::write, &time))
			return;

		for (auto& iter : _session) {
			if (!_select.is_set(iter._socket, network::select::write))
				continue;
			int size = (int)iter._send_ring_buffer.size();
			int deque = (int)iter._send_ring_buffer.at_once_send();
			size = size < deque ? size : deque;
			int result = iter._socket.send(
				(char*)(iter._send_ring_buffer.data() + iter._send_ring_buffer.get_front()),
				size, 0);
			if (SOCKET_ERROR != result)
				iter._send_ring_buffer.move_front(result);
		}
	}
	inline void close(void) noexcept {
		for (auto iter = _session.begin(); iter != _session.end();) {
			if (INVALID_SOCKET == (*iter)._socket.data()) {
				(*iter).close();
				iter = _session.erase(iter);
			}
			else
				++iter;
		}
	}
public:
	network::network _network;
	network::select _select;

	network::socket _listen;
	data_structure::list<session> _session;
};