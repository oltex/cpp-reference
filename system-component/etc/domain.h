#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "socket_address.h"
#include "../../data-structure/list/list.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace network::domain {
	inline static auto get_socket_address(wchar_t const* const node_name, wchar_t const* const service_name) noexcept -> library::data_structure::list<socket_address_storage> {
		library::data_structure::list<socket_address_storage> socket_address;

		addrinfoW* info;
		if (0 != GetAddrInfoW(node_name, service_name, nullptr, &info))
			__debugbreak();
		for (auto iter = info; nullptr != iter; iter = iter->ai_next) {
			sockaddr_storage sockaddr{};
			memcpy_s(&sockaddr, sizeof(sockaddr_storage), iter->ai_addr, iter->ai_addrlen);
			socket_address.emplace_back(sockaddr);
		}
		FreeAddrInfoW(info);

		return socket_address;
	}
}