#pragma once
#include "socket.h"
#include "storage.h"
#include "../../data-structure/ring-buffer/ring_buffer.h"

class session {
public:
	inline explicit session(network::socket&& socket, network::storage& storage)
		: _socket(std::move(socket)), _storage(storage) {
	}
	inline explicit session(session const& rhs) noexcept = delete;
	inline auto operator=(session const& rhs) noexcept -> session & = delete;
	inline explicit session(session&& rhs) noexcept = delete;
	inline auto operator=(session&& rhs) noexcept -> session & = delete;
	inline ~session(void) noexcept = default;
public:
	network::socket _socket;
	network::storage _storage;
	data_structure::ring_buffer _recv_ring_buffer;
	data_structure::ring_buffer _send_ring_buffer;
};