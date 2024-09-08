#pragma once
#include "socket.h"
#include "storage.h"
#include "ring_buffer.h"

class session {
public:
	inline explicit session(network::socket&& socket, network::storage& storage)
		: _socket(std::move(socket)), _storage(storage) {
	}
	inline ~session(void) noexcept = default;
public:
	inline void accept(void) noexcept {
	};
	inline void receive(void) noexcept {
	};
	inline void close(void) noexcept {
	};
public:
	network::socket _socket;
	network::storage _storage;
	data_structure::ring_buffer _recv_ring_buffer;
	data_structure::ring_buffer _send_ring_buffer;
};