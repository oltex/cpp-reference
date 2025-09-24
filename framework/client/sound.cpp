#include "sound.h"
#include "audio.h"

namespace framework {
	sound::sound(char const* const path, FMOD_MODE const mode) noexcept {
		//audio::instance()._system->createSound(path, mode, nullptr, &*_sound);
	}

	sound::sound(sound const& rhs) noexcept
		: _sound(rhs._sound) {
	}

	sound::~sound(void) noexcept {
	}
}