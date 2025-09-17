#pragma once
#include <Windows.h>
#include "library/function.h"

namespace winapi {
	class object {
	protected:
		HGDIOBJ _hgdiobj;
	public:
		inline explicit object(HGDIOBJ hgdiobj) noexcept
			: _hgdiobj(hgdiobj) {
		};
		inline explicit object(object const&) noexcept = delete;
		inline explicit object(object&& rhs) noexcept
			: _hgdiobj(library::exchange(rhs._hgdiobj, nullptr)) {
		};
		inline auto operator=(object const&) noexcept -> object & = delete;
		inline auto operator=(object&& rhs) noexcept -> object & = delete;
		inline virtual ~object(void) noexcept {
			if(nullptr != _hgdiobj)
			::DeleteObject(_hgdiobj);
		}

		inline auto data(void) noexcept -> HGDIOBJ& {
			return _hgdiobj;
		}
	};
}