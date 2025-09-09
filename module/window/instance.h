#pragma once
#include "library/pattern/singleton.h"
#include <Windows.h>

namespace window {
	class instance final : public library::singleton<instance, true> {
		friend class library::singleton<instance, true>;
		HINSTANCE _hinstance;
	public:
		inline explicit instance(HINSTANCE const hinstance) noexcept
			: _hinstance(hinstance) {
		}
		inline explicit instance(instance const&) noexcept = delete;
		inline explicit instance(instance&&) noexcept = delete;
		inline auto operator=(instance const&) noexcept -> instance & = delete;
		inline auto operator=(instance&&) noexcept -> instance & = delete;
		inline ~instance(void) noexcept = default;

		inline auto load_image(wchar_t const* const name, unsigned int const type, int const cx, int const cy, unsigned int const const fu_load) noexcept -> HANDLE {
			return ::LoadImageW(_hinstance, name, type, cy, cy, fu_load);
		}
		inline auto data(void) const noexcept -> HINSTANCE {
			return _hinstance;
		}
	};
}