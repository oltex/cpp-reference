#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>

namespace network {
	class storage_ipv4;
	class storage {
	public:
		inline explicit storage(void) noexcept
			:_storage{} {
		};
		inline explicit storage(sockaddr_storage const& storage) noexcept
			: _storage(storage) {
		}
		inline virtual ~storage(void) noexcept = default;
	public:
		inline auto get_family(void) const noexcept -> ADDRESS_FAMILY {
			return _storage.ss_family;
		}
		inline auto get_address(void) const noexcept -> std::string;
		inline auto data(void) noexcept -> sockaddr_storage& {
			return _storage;
		}
	protected:
		sockaddr_storage _storage;
	};

	class storage_ipv4 : protected storage {
	public:
		explicit storage_ipv4(void) noexcept {
			auto& storage = get_stroage();
			storage.sin_family = AF_INET;
		}
	public:
		inline void set_address(unsigned long address) noexcept {
			auto& storage = get_stroage();
			storage.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		}
		inline void set_address(char const* const address) noexcept {
			auto& storage = get_stroage();
			if (1 != inet_pton(AF_INET, address, &storage.sin_addr))
				DebugBreak();
		}
		inline auto get_address(void) const noexcept -> std::string {
			auto& storage = get_stroage();
			char string[INET_ADDRSTRLEN];
			if (0 == inet_ntop(AF_INET, &storage.sin_addr, string, INET_ADDRSTRLEN))
				DebugBreak();
			return std::string(string);
		}
		inline void set_port(unsigned short port) noexcept {
			auto& storage = get_stroage();
			storage.sin_port = htons(port);
		}
		inline auto get_port(void) noexcept -> unsigned short {
			auto& storage = get_stroage();
			return ntohs(storage.sin_port);
		}
	private:
		inline auto get_stroage(void) noexcept -> sockaddr_in& {
			return const_cast<sockaddr_in&>(const_cast<storage_ipv4 const&>(*this).get_stroage());
		}
		inline auto get_stroage(void) const noexcept -> sockaddr_in const& {
			return reinterpret_cast<sockaddr_in const&>(_storage);
		}
	};

	inline auto storage::get_address(void) const noexcept -> std::string {
		switch (get_family()) {
		case AF_INET:
			return reinterpret_cast<storage_ipv4 const&>(*this).get_address();
		}
	}
}