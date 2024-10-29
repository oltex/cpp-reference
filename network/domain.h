#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "storage.h"
#include "list.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace network {
	class domain final {
	public:
		inline explicit domain(void) noexcept = default;
		inline explicit domain(domain const& rhs) noexcept = delete;
		inline explicit domain(domain&& rhs) noexcept = delete;
		inline auto operator=(domain const& rhs) noexcept -> domain & = delete;
		inline auto operator=(domain&& rhs) noexcept -> domain & = delete;
		inline ~domain(void) noexcept = default;
	public:
		inline auto get_storage(wchar_t const* const node_name, wchar_t const* const service_name) const noexcept -> data_structure::list<storage> {
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
	};
}