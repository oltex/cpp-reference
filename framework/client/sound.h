#pragma once
#ifdef _DEBUG
#pragma comment(lib, "library/fmod/fmodL_vc.lib")
#else
#pragma comment(lib, "library/fmod/fmod_vc.lib")
#endif
#include "library/fmod/fmod.hpp"

namespace framework {
	class sound {
		FMOD::System* _system;
	public:
		explicit sound(void) noexcept;
		explicit sound(sound const&) noexcept = delete;
		explicit sound(sound&&) noexcept = delete;
		auto operator=(sound const&) noexcept -> sound & = delete;
		auto operator=(sound&&) noexcept -> sound & = delete;
		~sound(void) noexcept;
	};
}