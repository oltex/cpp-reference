#pragma once
#include "../memory.h"
#include "../template.h"
#include "../container/string.h"
#include <objbase.h>
#include <cassert>
#pragma comment(lib, "Rpcrt4.lib")
#include <rpc.h>

namespace detail {
	template<typename type, size_t sso>
		requires (library::any_of_type<type, char, wchar_t>)
	class string;
}

namespace library {

	class guid {
	public:
		GUID _guid;
		inline explicit guid(void) noexcept
			: _guid() {
		};
		inline guid(GUID guid) noexcept
			: _guid(guid) {
		};
		template<typename type>
		inline guid(detail::string<type> const& string) noexcept {
			RPC_STATUS result;
			if constexpr (library::same_type<char, type>)
				result = ::UuidFromStringA(reinterpret_cast<RPC_CSTR>(const_cast<char*>(string.data())), reinterpret_cast<UUID*>(&_guid));
			else
				result = ::UuidFromStringW(reinterpret_cast<RPC_WSTR>(const_cast<wchar_t*>(string.data())), reinterpret_cast<UUID*>(&_guid));
			assert(RPC_S_OK == result);
		}
		inline guid(guid const&) noexcept = default;
		inline guid(guid&&) noexcept = default;
		inline auto operator=(guid const&) noexcept -> guid & = default;
		inline auto operator=(guid&&) noexcept -> guid & = default;
		inline ~guid(void) noexcept = default;

		inline operator GUID const& (void) const noexcept {
			return _guid;
		}
		inline bool operator==(guid const& rhs) const noexcept {
			return 0 == library::memory_compare<GUID>(&_guid, &rhs._guid, 1);
		}
		inline bool operator<(guid const& rhs) const noexcept {
			return 0 > library::memory_compare<GUID>(&_guid, &rhs._guid, 1);
		}

		inline auto string(void) noexcept {
			char buffer[37];
			std::snprintf(buffer, sizeof(buffer),
				"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
				(unsigned)_guid.Data1, _guid.Data2, _guid.Data3,
				_guid.Data4[0], _guid.Data4[1], _guid.Data4[2], _guid.Data4[3],
				_guid.Data4[4], _guid.Data4[5], _guid.Data4[6], _guid.Data4[7]);

			return library::string(buffer);
		}
	};

	inline auto create_guid(void) noexcept -> guid {
		GUID _guid;
		auto result = ::CoCreateGuid(&_guid);
		assert(SUCCEEDED(result));
		return _guid;
	}
}