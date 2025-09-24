#include "sound.h"
#include "audio.h"

namespace framework {
	sound::sound(char const* const path, FMOD_MODE const mode) noexcept {
		FMOD::Sound* sound;
		audio::instance()._system->createSound(path, mode, nullptr, &sound);
		_sound = library::share_pointer<FMOD::Sound, sound_deleter>(sound);
	}

	sound::sound(sound const& rhs) noexcept
		: _sound(rhs._sound) {
	}

	sound::~sound(void) noexcept {
	}
}