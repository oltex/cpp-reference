#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

namespace network {
	enum class address_family : int { inet = AF_INET, inet6 = AF_INET6 };
	using enum address_family;
	enum class type : int { stream = SOCK_STREAM, datagram = SOCK_DGRAM };
	using enum type;
	enum class protocol : int { tcp = IPPROTO_TCP, udp = IPPROTO_UDP };
	using enum protocol;

	class address {
	public:
		inline explicit address(void) noexcept = default;
		inline virtual ~address(void) noexcept = default;
	public:
		inline auto get_family(void) noexcept -> ADDRESS_FAMILY {
			return _storage.ss_family;
		}
	protected:
		sockaddr_storage _storage{};
	};

	class address_ipv4 : protected address {
	public:
		explicit address_ipv4(void) noexcept {
			auto& storage = get_stroage();
			storage.sin_family = AF_INET;
		}
		explicit address_ipv4(sockaddr_in const storage) {
			memcpy_s(_storage, storage,
		}
	public:
		inline void set_port(unsigned short port) noexcept {
			auto& storage = get_stroage();
			storage.sin_port = htons(port);
		}
		inline void set_address(unsigned long address) noexcept {
			auto& storage = get_stroage();
			storage.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		}
		inline void set_address(char const* const address) noexcept {
			auto& storage = get_stroage();
			if (1 != inet_pton(AF_INET, address, &storage.sin_addr))
				std::cout << GetLastError() << std::endl;
		}
	private:
		inline auto get_stroage(void) noexcept -> sockaddr_in& {
			return *reinterpret_cast<sockaddr_in*>(&_storage);
		}
	};
}