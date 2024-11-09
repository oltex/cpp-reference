#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "storage.h"
#include "../../data-structure/list/list.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace network {
	namespace domain {
		inline static auto get_storage(wchar_t const* const node_name, wchar_t const* const service_name) noexcept -> data_structure::list<storage> {
			data_structure::list<storage> storage;

			addrinfoW* info;
			if (0 != GetAddrInfoW(node_name, service_name, nullptr, &info))
				__debugbreak();
			for (auto iter = info; nullptr != iter; iter = iter->ai_next) {
				sockaddr_storage addr{};
				memcpy_s(&addr, sizeof(sockaddr_storage), iter->ai_addr, iter->ai_addrlen);
				storage.emplace_back(addr);
			}
			FreeAddrInfoW(info);

			return storage;
		}
	}
}