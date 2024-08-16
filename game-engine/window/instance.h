#pragma once
#include <Windows.h>

namespace window {
	class instance final {
	public:
		inline explicit instance(HINSTANCE const hinstance) noexcept
			: _hinstance(hinstance) {
		}
		inline ~instance(void) noexcept = default;
	public:
		inline auto data(void) const noexcept -> HINSTANCE {
			return _hinstance;
		}
	private:
		HINSTANCE _hinstance;
	};
}