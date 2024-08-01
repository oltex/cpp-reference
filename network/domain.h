#pragma once
#pragma comment(lib,"ws2_32.lib")
#include "address.h"
#include "list.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace network {
	class domain final {
	public:
		inline void get_address(wchar_t const* const node_name, wchar_t const* const service_name) noexcept {
			addrinfoW* info;
			if (0 != GetAddrInfoW(node_name, service_name, nullptr, &info)) {
				std::cout << GetLastError() << std::endl;
				DebugBreak();
			}

			for (addrinfoW* iter = info; nullptr != iter; iter = iter->ai_next) {
				
				switch (iter->ai_family) {
					case 
					_address.emplace();
				}
				sockaddr_in* socketaddrin = reinterpret_cast<sockaddr_in*>(iter->ai_addr);

				char str[INET_ADDRSTRLEN];
				if (0 == inet_ntop(socketaddrin->sin_family, &socketaddrin->sin_addr, str, INET_ADDRSTRLEN))
					std::cout << GetLastError() << std::endl;

				std::cout << str << std::endl;
			}
		}
	private:
		list<address> _address;
	};
}