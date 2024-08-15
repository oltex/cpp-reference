#pragma once
#include <Windows.h>

namespace window {
	class object {
	public:
		inline explicit object(void) noexcept = default;
		virtual ~object(void) noexcept {
			DeleteObject(_hgdiobj);
		}
	public:
		inline auto data(void) const noexcept -> HGDIOBJ {
			return _hgdiobj;
		}
	protected:
		HGDIOBJ _hgdiobj = nullptr;
	};
}

