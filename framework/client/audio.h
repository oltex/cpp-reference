#pragma once
#include "library/pattern/singleton.h"
#ifdef _DEBUG
#pragma comment(lib, "library/fmod/fmodL_vc.lib")
#else
#pragma comment(lib, "library/fmod/fmod_vc.lib")
#endif
#include "library/fmod/fmod.hpp"

namespace framework {
	class sound;
	class audio : public library::singleton<audio> {
		friend class library::singleton<audio>;
		friend class sound;
		FMOD::System* _system;
	public:
		explicit audio(void) noexcept;
		explicit audio(audio const&) noexcept = delete;
		explicit audio(audio&&) noexcept = delete;
		auto operator=(audio const&) noexcept -> audio & = delete;
		auto operator=(audio&&) noexcept -> audio & = delete;
		~audio(void) noexcept;

		void update_sound(void) noexcept;
	};
}