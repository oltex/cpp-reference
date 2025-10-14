#pragma once
#include "../memory.h"
#include <objbase.h>
#include <cassert>

namespace library {
	class guid {
		GUID _guid;
	public:
		inline explicit guid(void) noexcept
			: _guid() {
		};
		inline guid(GUID guid) noexcept
			: _guid(guid) {
		};
		inline guid(guid const&) noexcept = default;
		inline guid(guid&&) noexcept = default;
		inline auto operator=(guid const&) noexcept -> guid & = default;
		inline auto operator=(guid&&) noexcept -> guid & = default;
		inline ~guid(void) noexcept = default;

		inline operator GUID const& (void) const noexcept {
			return _guid;
		}
		inline bool operator==(guid const& rhs) const noexcept {
			return 0 == library::memory_compare(&_guid, &rhs._guid, sizeof(GUID));
		}
		inline bool operator<(guid const& rhs) const noexcept {
			return 0 > library::memory_compare(&_guid, &rhs._guid, sizeof(GUID));
		}
	};

	inline auto create_guid(void) noexcept -> guid {
		GUID _guid;
		auto result = CoCreateGuid(&_guid);
		assert(SUCCEEDED(result));
		return _guid;
	}
}